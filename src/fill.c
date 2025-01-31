/* 
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * This file contains all the read and write functions for the
 * library.
 *
 * FILL ALGORITHMS
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "embroidery_internal.h"

const char *rules[] = {"+BF-AFA-FB+", "-AF+BFB+FA-"};

L_system hilbert_curve_l_system = {
    'A', "AB", "F+-", (char**)rules
};

/**
 * \a L \a state \a iterations \a complete 
 * @return int 
 *
 * This is a slow generation algorithm.
 */
int lindenmayer_system(L_system L, char *state, int iterations, int complete)
{
    /* We know that the full length of the curve has to fit within
     * the number of stitches and we can cancel consecutive +-, -+
     * etc.
     *

Potential reference:

@book{Prusinkiewicz1996Mar,
    author = {Prusinkiewicz, Przemyslaw and Lindenmayer, Aristid and Hanan, J. S. and Fracchia, F. D. and Fowler, D. R. and de Boer, M. J. M. and Mercer, L.},
    title = {{The Algorithmic Beauty of Plants (The Virtual Laboratory)}},
    year = {1996},
    month = {Mar},
    publisher = {Springer}
}
     */
    char *new_state;
    int j;

    if (complete == 0) {
        state[0] = L.axiom;
        state[1] = 0;
        lindenmayer_system(L, state, iterations, complete+1);
        return 0;
    }

    new_state = state + MAX_STITCHES*5;

    new_state[0] = 0;

    /* replace letters using rules by copying to new_state */
    for (j=0; j < (int)strlen(state); j++) {
        if (state[j] >= 'A' && state[j] < 'F') {
            strcat(new_state, L.rules[state[j]-'A']);
        }
        if (state[j] == 'F') {
            strcat(new_state, "F");
        }
        if (state[j] == '+') {
            strcat(new_state, "+");
        }
        if (state[j] == '-') {
            strcat(new_state, "-");
        }
    }
    memcpy(state, new_state, strlen(new_state)+1);

    if (complete < iterations) {
        lindenmayer_system(L, state, iterations, complete+1);
    }
    return 0;
}

/**
 * \a points \a n_points \a width \a tolerence 
 *
 * Remove points that lie in the middle of two short stitches that could
 * be one longer stitch. Repeat until none are found.
 */
static void
join_short_stitches(int *points, int *n_points, int width, int tolerence)
{
    int found = 1;
    while (found > 0) {
        int i;
        found = 0;
        for (i=*n_points-2; i>=0; i--) {
            int st1 = points[i+1]%width - points[i]%width;
            int st2 = points[i+2]%width - points[i+1]%width;
            int same_line = (points[i+1]/width == points[i]/width)
                            && (points[i+2]/width == points[i+1]/width);
            if (st1 < tolerence && st2 < tolerence && same_line) {
                found++;
                break;
            }
        }
        if (found) {
            /* Remove the point. */
            i++;
            for (; i<*n_points; i++) {
                points[i] = points[i+1];
            }
            (*n_points)--;
        }
    }
}

/**
 * \a image \a n_points \a subsample_width \a subsample_height 
 * \a threshold 
 * @return int* 
 *
 * Identify darker pixels to put stitches in.
 */
static int *
threshold_method(EmbImage *image, int *n_points,
    int subsample_width, int subsample_height, int threshold)
{
    int i, j;
    int *points;
    int height = image->height;
    int width = image->width;
    points = (int *)malloc((height/subsample_height)
        *(width/subsample_width) * sizeof(int));
    *n_points = 0;
    for (i=0; i<height/subsample_height; i++)
    for (j=0; j<width/subsample_width; j++) {
        EmbColor color;
        int index = subsample_height*i*width+subsample_width*j;
        color.r = image->data[3*index+0];
        color.g = image->data[3*index+1];
        color.b = image->data[3*index+2];
        if (color.r+color.g+color.b < threshold) {
            points[*n_points] = index;
            (*n_points)++;
        }
    }
    return points;
}

/**
 * \a points \a n_points \a width \a bias 
 *
 * Greedy Algorithm
 * ----------------
 * For each point in the list find the shortest distance to
 * any possible neighbour, then perform a swap to make that
 * neighbour the next item in the list.
 * 
 * To make the stitches lie more on one axis than the other
 * bias the distance operator to prefer horizontal direction.
 */
static void
greedy_algorithm(int *points, int n_points, int width, EmbReal bias)
{
    int i, j;
    printf("points[0] = %d\n", points[0]);
    printf("n_points = %d\n", n_points);
    printf("width = %d\n", width);
    printf("bias = %f\n", bias);

    for (i=0; i<n_points-1; i++) {
        int stor;
        EmbReal shortest = 1.0e20;
        int next = i+1;
        /* Find nearest neighbour. */
        int x1 = points[i]%width;
        int y1 = points[i]/width;
        for (j=i+1; j<n_points; j++) {
            int x, y;
            EmbReal distance;
            x = x1 - (points[j]%width);
            if (x*x > shortest) {
                continue;
            }
            y = y1 - (points[j]/width);
            distance = x*x + bias*y*y;
            if (distance < shortest) {
                next = j;
                shortest = distance;
            }
        }
        if (i%100 == 0) {
            printf("%2.1f%%\n", (100.0*i)/(1.0*n_points));
        }
        /* swap points */
        stor = points[next];
        points[next] = points[i+1];
        points[i+1] = stor;
    }
}

/**
 * \a pattern \a points \a n_points 
 * \a scale \a width \a height 
 */
static void
save_points_to_pattern(
    EmbPattern *pattern, int *points, int n_points, EmbReal scale, int width, int height)
{
    int i;
    for (i=0; i<n_points; i++) {
        int x, y;
        x = points[i]%width;
        y = height - points[i]/width;
        embPattern_addStitchAbs(pattern, scale*x, scale*y, NORMAL, 0);
    }
}

/**
 * \a pattern \a image \a threshhold 
 *
 * Uses a threshhold method to determine where to put
 * lines in the fill.
 *
 * Needs to pass a "donut test", i.e. an image with black pixels where:
 *     10 < x*x + y*y < 20
 * over the area (-30, 30) x (-30, 30).
 * 
 * Use render then image difference to see how well it passes.
 */
void
embPattern_horizontal_fill(EmbPattern *pattern, EmbImage *image, int threshhold)
{
    /* Size of the crosses in millimeters. */
    EmbReal scale = 0.1;
    int sample_w = 3;
    int sample_h = 3;
    EmbReal bias = 1.2;
    int *points;
    int n_points;

    points = threshold_method(image, &n_points, sample_w, sample_h, threshhold);
    greedy_algorithm(points, n_points, image->width, bias);
    join_short_stitches(points, &n_points, image->width, 40);
    save_points_to_pattern(pattern, points, n_points, scale, image->width, image->height);

    embPattern_end(pattern);
    free(points);
}

/**
 * \a pattern \a image \a threshhold 
 *
 * Uses a threshhold method to determine where to put
 * crosses in the fill.
 *
 * To improve this, we can remove the vertical stitches when two crosses
 * neighbour. Currently the simple way to do this is to chain crosses
 * that are neighbours exactly one ahead.
 */
void
embPattern_crossstitch(EmbPattern *pattern, EmbImage *image, int threshhold)
{
    int i;
    /* Size of the crosses in millimeters. */
    EmbReal scale = 0.1;
    int sample_w = 5;
    int sample_h = 5;
    EmbReal bias = 1.0;
    int *points;
    int n_points;
    int width = 1000;
    points = threshold_method(image, &n_points, sample_w, sample_h, threshhold);
    greedy_algorithm(points, n_points, width, bias);

    for (i=0; i<n_points; i++) {
        EmbReal x, y;
        x = points[i]%width;
        y = points[i]/width;
        printf("%f %f\n", x, y);
        embPattern_addStitchAbs(pattern, scale*x, scale*y, NORMAL, 0);
        embPattern_addStitchAbs(pattern, scale*(x+sample_w), scale*(y+sample_h), NORMAL, 0);
        embPattern_addStitchAbs(pattern, scale*x, scale*(y+sample_h), NORMAL, 0);
        embPattern_addStitchAbs(pattern, scale*(x+sample_w), scale*y, NORMAL, 0);
    }

    embPattern_end(pattern);
}

/**
 * \a pattern \a iterations 
 *
 * https://en.wikipedia.org/wiki/Hilbert_curve
 *
 * Using the Lindenmayer System, so we can save work across
 * different functions.
 */
int
hilbert_curve(EmbPattern *pattern, int iterations)
{
    char *state;
    int i, position[2], direction;
    EmbReal scale = 1.0;

    /* Make the n-th iteration. */
    state = malloc(MAX_STITCHES*10);
    lindenmayer_system(hilbert_curve_l_system, state, iterations, 0);

    /* Convert to an embroidery pattern. */
    position[0] = 0;
    position[1] = 0;
    direction = 0;

    for (i = 0; i < (int)strlen(state); i++) {
        if (state[i] == '+') {
            direction = (direction + 1) % 4;
            continue;
        }
        if (state[i] == '-') {
            direction = (direction + 3) % 4;
            continue;
        }
        if (state[i] == 'F') {
            int flags = NORMAL;
            switch (direction) {
            case 0:
            default:
                position[0]--;
                break;
            case 1:
                position[1]++;
                break;
            case 2:
                position[0]++;
                break;
            case 3:
                position[1]--;
                break;
            }
            embPattern_addStitchAbs(pattern, position[0]*scale, position[1]*scale, flags, 0);
        }
    }
    free(state);
    embPattern_end(pattern);
    return 0;
}

/**
 * \a state \a iterations 
 *
 * using the "paper folding" method
 * \todo find citation for paper folding method
 */
void generate_dragon_curve(char *state, int iterations)
{
    int i, length;
    if (iterations == 1) {
        state[0] = 'R';
        state[1] = 0;
        return;
    }
    length = strlen(state);
    for (i=length-1; i>=0; i--) {
        state[2*i+1] = state[i];
        if (i%2 == 0) {
            state[2*i] = 'R';
        } else {
            state[2*i] = 'L';
        }
    }
    state[2*length+1] = 0;
    generate_dragon_curve(state, iterations-1);
}

/**
 * Create the dragon curve for \a iterations.
 *
 * Returns 0 if the number of iterations is greater than 10
 * and 1 otherwise. 
 */
int
dragon_curve(int iterations)
{
    char *state;
    if (iterations > 10) {
        puts("The dragon curve is only supported up to 10 iterations.");
        return 0;
    }
    state = malloc(1<<(iterations+1));
    generate_dragon_curve(state, iterations);
    free(state);
    return 1;
}

#if 0
/**
 * \a pattern 
 * @return StitchBlock* 
 */
StitchBlock*
BreakIntoColorBlocks(EmbPattern *pattern)
{
    int i;
    int sa2 = new StitchBlock();
    int oldColor = pattern->stitch_list->stitch[0].color;
    int color = pattern.ColorList[oldColor];
    sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
    for (i = 0; i < pattern->stitch_list->count; i++) {
        EmbStitch s = pattern->stitch_list->stitch[i];
        if (s.color != oldColor) {
            yield return sa2;
            sa2 = new StitchBlock();
            color = pattern.ColorList[s.ColorIndex];
            sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
            oldColor = s.ColorIndex;
        }
        int vs = new VectorStitch { Xy = new Point(s.X, s.Y), Color = s.ColorIndex };
        sa2.Stitches.Add(vs);
    }
    yield return sa2;
}

/**
 * \a blocks 
 * @return StitchBlock* 
 */
StitchBlock *
BreakIntoSeparateObjects(EmbStitchBlock* blocks)
{
    int i, block;
    EmbReal previousAngle = 0.0;
    for (block=0; block<blocks->length; block++) {
        int stitches = new List<VectorStitch>();
        block.Stitches[0].Type = VectorStitchType.Contour;
        block.Stitches[block.Stitches.Count - 1].Type = VectorStitchType.Contour;

        for (int i = 0; i < block.Stitches.Count - 2; i++) { /* step 0 */
            EmbReal dx = (embVector_relativeX(block.Stitches[i].Xy, block.Stitches[i + 1].Xy, block.Stitches[i + 2].Xy));
            block.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
            block.Stitches[i].Angle = GetAngle(block.Stitches[i], block.Stitches[i + 1]);
            stitches.Add(block.Stitches[i].Clone());
            if (i > 0) {
                if ((block.Stitches[i].Type == VectorStitchType.Contour) && fabs(block.Stitches[i].Angle - previousAngle) > (20/180*embConstantPi)) {
                    yield return
                        new StitchBlock
                            {
                                Stitches = stitches,
                                Angle = stitches.Average(x => x.Angle),
                                Thread = new Thread(block.Thread.Red, block.Thread.Blue, block.Thread.Green)
                            };
                    stitches = new List<VectorStitch>();

                }
            }
        }

        /* step 1 */
        for (i = 1; i < sa.Stitches.Count - 3; i++) {
            if (sa.Stitches[i + 1].Type == VectorStitchType.Contour) {
                float dy = embVector_relativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
                float dy2 = embVector_relativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
                float dy3 = embVector_relativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
                if (dy)
                if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run) {
                    sa.Stitches[i].Type = VectorStitchType.Tatami;
                }
                else {
                    sa.Stitches[i].Type = VectorStitchType.Satin;
                }
            }
        }
    }
}

/**
 * \a stitchData 
 * @return StitchObject* 
 */
StitchObject *
FindOutline(EmbStitchBlock* stitchData)
{
    int currColorIndex = 0, sa;
    int pOdd = new List<Point>();
    int pEven = new List<Point>();
    for (sa=0; sa<stitchData->count; sa++) {
        if (sa.Stitches.Count > 0) {
            sa.Stitches[0].Type = VectorStitchType.Contour;
            sa.Stitches[sa.Stitches.Count - 1].Type = VectorStitchType.Contour;
            /* step 0 */
            for (int i = 0; i < sa.Stitches.Count - 2; i++) {
                float dx = (GetRelativeX(sa.Stitches[i].Xy, sa.Stitches[i + 1].Xy, sa.Stitches[i + 2].Xy));
                sa.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
                sa.Stitches[i].Angle = embVector_angle(sa.Stitches[i], sa.Stitches[i + 1]);
            }
            /* step 1 */
            for (int i = 1; i < sa.Stitches.Count - 3; i++) {
                if (sa.Stitches[i + 1].Type == VectorStitchType.Contour) {
                    float dy = embVector_relativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
                    float dy2 = embVector_relativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
                    float dy3 = embVector_relativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
                    if (dy)
                    if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run) {
                        sa.Stitches[i].Type = VectorStitchType.Tatami;
                    }
                    else {
                        sa.Stitches[i].Type = VectorStitchType.Satin;
                    }
                }
            }
        }


        int oddEven = 0;
        foreach (VectorStitch t in sa.Stitches) {
            if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 0) {
                pEven.Add(t.Xy);

                oddEven++;
            }
            else if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 1) {
                pOdd.Add(t.Xy);
                oddEven++;
            }
        }
        currColorIndex++;
        int so = new StitchObject { SideOne = pEven, SideTwo = pOdd, ColorIndex = currColorIndex };
        yield return so;
        pEven = new List<Point>();
        pOdd = new List<Point>();
        /* break; */
    }
}

/**
 * \a p 
 * @return EmbPattern 
 */
EmbPattern
DrawGraphics(EmbPattern p)
{
    int stitchData = BreakIntoColorBlocks(p);

    int outBlock = new List<StitchBlock>(BreakIntoSeparateObjects(stitchData));
    foreach(var block in stitchData) {
        foreach (var stitch in block.Stitches) {
            if (stitch.Angle != 0) {
                int aaa = 1;
            }
        }
    }
    int xxxxx = outBlock;
    int objectsFound = FindOutline(stitchData);
    int outPattern = new Pattern();
    outPattern.AddColor(new Thread(255, 0, 0, "none", "None"));
    int colorIndex = outPattern.ColorList.Count - 1;
    int r = new Random();
    foreach (StitchObject stitchObject in objectsFound) {
        if (stitchObject.SideOne.Count > 1 && stitchObject.SideTwo.Count > 1) {
            outPattern.AddColor(new Thread((byte) (r.Next()%256), (byte) (r.Next()%256), (byte) (r.Next()%256),
                                           "none", "None"));
            colorIndex++;
            outPattern.AddStitchRelative(0, 0, StitchTypes.Stop);
            int points = stitchObject.Generate2(75);
            foreach (var point in points) {
                outPattern.AddStitchAbsolute(point.X, point.Y, StitchTypes.Normal);
            }
            break;
            StitchObject stitchObject = objectsFound[1];))
            if (stitchObject.SideOne.Count > 0) {
                outPattern.stitch_list.Add(new Stitch(stitchObject.SideOne[0].X, stitchObject.SideOne[0].Y,
                                                     StitchType.Jump, colorIndex));
            }
            foreach (Point t in stitchObject.SideOne) {
            
                outPattern.stitch_list.Add(new Stitch(t.X, t.Y,
                                                     StitchType.Normal, colorIndex));
            }
            foreach (Point t in stitchObject.SideTwo) {
                outPattern.stitch_list.Add(new Stitch(t.X, t.Y,
                                                     StitchType.Normal, colorIndex));
            }
            break;
        }
    }
    outPattern.AddStitchRelative(0, 0, StitchTypes.End);
    return outPattern;
    /*
    return (SimplifyOutline(outPattern));
    */
}

/**
 * @brief 
 * 
 * \a pattern 
 * @return EmbPattern 
 */
EmbPattern
SimplifyOutline(EmbPattern pattern)
{
    int v = new Vertices();
    v.AddRange(pattern.stitch_list.Select(point => new Vector2(point.X, point.Y)));
    int output = SimplifyTools.DouglasPeuckerSimplify(v, 10);
    int patternOut = new Pattern();
    foreach (var color in pattern.ColorList)
    {
        patternOut.AddColor(color);
    }

    foreach (var vertex in output)
    {
        patternOut.AddStitchAbsolute(vertex.X, vertex.Y, StitchTypes.Normal);
    }
    patternOut.AddStitchRelative(0, 0, StitchTypes.End);
    return patternOut;
}

bool[] _usePt;
EmbReal _distanceTolerance;

/**
 * @brief Removes all collinear points on the polygon.
 * 
 * \a vertices 
 * \a collinearityTolerance 
 * @return Vertices 
 */
Vertices CollinearSimplify(Vertices vertices, float collinearityTolerance)
{
    /* We can't simplify polygons under 3 vertices */
    if (vertices.Count < 3)
        return vertices;

    int simplified = new Vertices();

    for (int i = 0; i < vertices.Count; i++) {
        int prevId = vertices.PreviousIndex(i);
        int nextId = vertices.NextIndex(i);

        Vector2 prev = vertices[prevId];
        Vector2 current = vertices[i];
        Vector2 next = vertices[nextId];

        /* If they collinear, continue */
        if (embVector_collinear(ref prev, ref current, ref next, collinearityTolerance))
            continue;

        simplified.Add(current);
    }

    return simplified;
}

/**
 * @brief 
 * 
 * \a vertices 
 * @return Vertices 
 *
 * Removes all collinear points on the polygon. Has a default bias of 0.
 *
 * param vertices: The polygon that needs simplification.
 * returns: A simplified polygon.
 */
Vertices
CollinearSimplify(Vertices vertices)
{
    return CollinearSimplify(vertices, 0);
}

/**
 * @brief 
 * 
 * \a vertices 
 * \a distanceTolerance 
 * @return Vertices 
 *
 * Ramer-Douglas-Peucker polygon simplification algorithm.
 * This is the general recursive version that does not use the
 * speed-up technique by using the Melkman convex hull.
 * If you pass in 0, it will remove all collinear points.
 *
 * \todo Ramer-Douglas-Peucker citation
 */
Vertices
DouglasPeuckerSimplify(Vertices vertices, float distanceTolerance)
{
    _distanceTolerance = distanceTolerance;

    _usePt = new bool[vertices.Count];
    for (int i = 0; i < vertices.Count; i++)
    {
        _usePt[i] = true;
    }
    SimplifySection(vertices, 0, vertices.Count - 1);
    int result = new Vertices();
    result.AddRange(vertices.Where((t, i) => _usePt[i]));
    return result;
}

/**
 * \a vertices \a i \a j 
 */
void
SimplifySection(Vertices vertices, int i, int j)
{
    if ((i + 1) == j)
        return;

    Vector2 a = vertices[i];
    Vector2 b = vertices[j];
    EmbReal maxDistance = -1.0;
    int maxIndex = i;
    for (int k = i + 1; k < j; k++)
    {
        EmbReal distance = DistancePointLine(vertices[k], a, b);

        if (distance > maxDistance)
        {
            maxDistance = distance;
            maxIndex = k;
        }
    }
    if (maxDistance <= _distanceTolerance) {
        for (int k = i + 1; k < j; k++) {
            _usePt[k] = 0;
        }
    }
    else {
        SimplifySection(vertices, i, maxIndex);
        SimplifySection(vertices, maxIndex, j);
    }
}

/**
 * \a p \a a \a b 
 * @return EmbReal 
 */
EmbReal
DistancePointLine(EmbVector p, EmbVector a, EmbVector b)
{
    /* if start == end, then use point-to-point distance */
    if (a.X == b.X && a.Y == b.Y)
        return DistancePointPoint(p, a);

    /* otherwise use comp.graphics.algorithms Frequently Asked Questions method */
    /* (1)               AC dot AB
                r =   ---------
                      ||AB||^2

                r has the following meaning:
                r=0 Point = A
                r=1 Point = B
                r<0 Point is on the backward extension of AB
                r>1 Point is on the forward extension of AB
                0<r<1 Point is interior to AB
    */

    EmbReal r = ((p.X - a.X) * (b.X - a.X) + (p.Y - a.Y) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    if (r <= 0.0) return DistancePointPoint(p, a);
    if (r >= 1.0) return DistancePointPoint(p, b);


    /* (2)
                    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
                s = -----------------------------
                                Curve^2

                Then the distance from C to Point = |s|*Curve.
    */

    EmbReal s = ((a.Y - p.Y) * (b.X - a.X) - (a.X - p.X) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    return fabs(s) * sqrt(((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y)));
}

/**
 * \a vertices \a areaTolerance 
 * @return public 
 *
 * From physics2d.net.
 */
public Vertices
ReduceByArea(Vertices vertices, float areaTolerance)
{
    if (vertices.Count <= 3)
        return vertices;

    if (areaTolerance < 0)
    {
        throw new ArgumentOutOfRangeException("areaTolerance", "must be equal to or greater then zero.");
    }

    int result = new Vertices();
    Vector2 v3;
    Vector2 v1 = vertices[vertices.Count - 2];
    Vector2 v2 = vertices[vertices.Count - 1];
    areaTolerance *= 2;
    for (int index = 0; index < vertices.Count; ++index, v2 = v3)
    {
        if (index == vertices.Count - 1)
        {
            if (result.Count == 0)
            {
                throw new ArgumentOutOfRangeException("areaTolerance", "The tolerance is too high!");
            }
            v3 = result[0];
        }
        else
        {
            v3 = vertices[index];
        }
        float old1, old2, new1;
        MathUtils.Cross(ref v1, ref v2, out old1);
        MathUtils.Cross(ref v2, ref v3, out old2);
        MathUtils.Cross(ref v1, ref v3, out new1);
        if (fabs(new1 - (old1 + old2)) > areaTolerance)
        {
            result.Add(v2);
            v1 = v2;
        }
    }
    return result;
}

/**
 * \a vertices \a tolerance 
 *
 * From Eric Jordan's convex decomposition library.
 * Merges all parallel edges in the list of vertices.
 */
void
MergeParallelEdges(EmbArray *vertices, float tolerance)
{
    int i;
    if (vertices.Count <= 3) {
        /* Can't do anything useful here to a triangle. */
        return;
    }

    int mergeMe = new bool[vertices.Count];
    int newNVertices = vertices.Count;

    /* Gather points to process */
    for (i = 0; i < vertices->count; i++) {
        EmbVector delta0, delta1;
        int lower = (i == 0) ? (vertices.Count - 1) : (i - 1);
        int upper = (i == vertices.Count - 1) ? (0) : (i + 1);

        delta0 = embVector_subtract(vertices[i], vertices[lower]);
        delta1 = embVector_subtract(vertices[upper], vertices[i]);
        float norm0 = embVector_length(delta0);
        float norm1 = embVector_length(delta0);

        if (!(norm0 > 0.0f && norm1 > 0.0f) && newNVertices > 3) {
            /* Merge identical points */
            mergeMe[i] = 1;
            newNVertices--;
        }

        embVector_normalize(delta0, &delta0);
        embVector_normalize(delta1, &delta1);
        float cross = embVector_cross(delta0, delta1);
        float dot = embVector_dot(delta0, delta1);

        if (fabs(cross) < tolerance && dot > 0 && newNVertices > 3) {
            mergeMe[i] = 1;
            newNVertices--;
        }
        else {
            mergeMe[i] = 0;
        }
    }

    if (newNVertices == vertices.Count || newNVertices == 0)
        return;

    int currIndex = 0;

    /* Copy the vertices to a new list and clear the old */
    int oldVertices = new Vertices(vertices);
    vertices.Clear();

    for (i = 0; i < oldVertices.Count; i++) {
        if (mergeMe[i] || newNVertices == 0 || currIndex == newNVertices)
            continue;

        vertices.Add(oldVertices[i]);
        currIndex++;
    }
}
#endif

void embPolygon_reduceByDistance(EmbArray *vertices, EmbArray *simplified, float distance);
void embPolygon_reduceByNth(EmbArray *vertices, EmbArray *out, int nth);

/**
 * \a vertices \a simplified \a distance 
 *
 * Reduces the polygon by distance.
 *
 * This is a non-destructive function, so the caller is responsible for
 * freeing "vertices" if they choose to keep "simplified".
 */
void embPolygon_reduceByDistance(EmbArray *vertices, EmbArray *simplified, float distance)
{
    int i;
    /* We can't simplify polygons under 3 vertices */
    if (vertices->count < 3) {
        embArray_copy(simplified, vertices);
        return;
    }

    for (i = 0; i < vertices->count; i++) {
        EmbVector delta;
        int nextId = (i + 1) % vertices->count;

        delta = embVector_subtract(
            vertices->geometry[nextId].object.vector,
            vertices->geometry[i].object.vector);

        /* If they are closer than the distance, continue */
        if (embVector_length(delta) < distance) {
            continue;
        }

        embArray_addVector(simplified, vertices->geometry[i].object.vector);
    }
}

/**
 * \a vertices \a out \a nth 
 *
 * Reduces the polygon by removing the Nth vertex in the vertices list.
 * This is a non-destructive function, so the caller is responsible for
 * freeing vertices if they choose to keep out.
 */
void
embPolygon_reduceByNth(EmbArray *vertices, EmbArray *out, int nth)
{
    int i;
    /* We can't simplify polygons under 3 vertices */
    if (vertices->count < 3) {
        embArray_copy(out, vertices);
        return;
    }

    for (i=0; i<vertices->count; i++) {
        if (i!=nth) {
            embArray_addVector(out, vertices->geometry[i].object.vector);
        }
    }
}

/**
 * \a p1 \a p2 
 * @return EmbPattern* 
 */
EmbPattern *
embPattern_combine(EmbPattern *p1, EmbPattern *p2)
{
    int i;
    EmbPattern *out = embPattern_create();
    for (i=0; i<p1->stitch_list->count; i++) {
        embArray_addStitch(out->stitch_list, p1->stitch_list->stitch[i]);
    }
    for (i=0; i<p2->stitch_list->count; i++) {
        embArray_addStitch(out->stitch_list, p2->stitch_list->stitch[i]);
    }
    /* These need to be merged, not appended. */
    for (i=0; i<p1->thread_list->count; i++) {
        embPattern_addThread(out, p1->thread_list->thread[i]);
    }
    for (i=0; i<p2->thread_list->count; i++) {
        embPattern_addThread(out, p2->thread_list->thread[i]);
    }
    return out;
}

/** 
 * \a p \a arc \a thread_index \a style 
 */
void
embPattern_stitchArc(EmbPattern *p, EmbArc arc, int thread_index, int style)
{
    printf("DEBUG stitchArc (unfinished): %f %f %d %d\n",
        p->home.x, arc.start.x, thread_index, style);
}

/**
 * \a p \a circle \a thread_index \a style 
 *
 * style determines:
 *     stitch density
 *     fill pattern
 *     outline or fill
 *
 * For now it's a straight fill of 1000 stitches of the whole object by 
 * default.
 *
 * Consider the intersection of a line in direction "d" that passes through
 * the disc with center "c", radius "r". The start and end points are:
 *
 *     $(c-r(d/|d|), c + r(d/|d|))$
 *
 * Lines that are above and below this with an even seperation $s$ can be 
 * found by taking the point on the line to be c+sn where the $n$ is the 
 * unit normal vector to $d$ and the vector to be $d$ again. The
 * intersection points are therefore a right angled triangle, with one side
 * r, another s and the third the length to be solved, by Pythagoras we
 * have:
 *
 *    $(c + sn - \sqrt{r^2-s^2}(d/|d|), c + sn + \sqrt{r^2-s^2}(d/|d|))$
 *
 * repeating this process gives us all the end points and the fill only 
 * alters these lines by splitting the ones longer than some tolerence.
 */
void
embPattern_stitchCircle(EmbPattern *p, EmbCircle circle, int thread_index, int style)
{
    float s;
    float seperation = 0.1;
    EmbVector direction = {1.0, 1.0};
    EmbVector normal = {-1.0, 1.0};
    embVector_normalize(direction, &direction);
    embVector_normalize(normal, &normal);
    for (s=-circle.radius; s<circle.radius; s += seperation) {
        EmbLine line;
        float length = sqrt(circle.radius*circle.radius - s*s);
        EmbVector scaled;
        embVector_multiply(normal, s, &scaled);
        line.start = embVector_add(circle.center, scaled);
        embVector_multiply(direction, length, &scaled);
        line.start = embVector_subtract(line.start, scaled);
        embVector_multiply(normal, s, &scaled);
        line.end = embVector_add(circle.center, scaled);
        embVector_multiply(direction, length, &scaled);
        line.end = embVector_add(line.end, scaled);
        /* Split long stitches here. */
        embPattern_addStitchAbs(p, line.start.x, line.start.y, NORMAL, thread_index);
        embPattern_addStitchAbs(p, line.end.x, line.end.y, NORMAL, thread_index);
    }
}

/**
 * \a p \a ellipse \a thread_index \a style 
 *
 * \todo finish stitchEllipse
 */
void
embPattern_stitchEllipse(EmbPattern *p, EmbEllipse ellipse, int thread_index, int style)
{
    printf("DEBUG stitchEllipse: %f %f %d %d\n",
        p->home.x, ellipse.center.x, thread_index, style);
}

/**
 * \a p \a rect \a thread_index \a style 
 *
 * \todo finish stitch path
 */
void
embPattern_stitchPath(EmbPattern *p, EmbPath path, int thread_index, int style)
{
    printf("DEBUG stitchPath: %f %d %d %d\n",
        p->home.x, path.pointList->count, thread_index, style);
}

/**
 * \a p \a rect \a thread_index \a style 
 *
 * \todo finish stitch polygon
 */
void
embPattern_stitchPolygon(EmbPattern *p, EmbPolygon polygon, int thread_index, int style)
{
    printf("DEBUG stitchPolygon: %f %d %d %d\n",
        p->home.x, polygon.pointList->count, thread_index, style);
}

/**
 * \a p \a rect \a thread_index \a style 
 *
 * \todo finish stitch polyline
 */
void
embPattern_stitchPolyline(EmbPattern *p, EmbPolyline polyline, int thread_index, int style)
{
    printf("DEBUG stitchPolyline: %f %d %d %d\n",
        p->home.x, polyline.pointList->count, thread_index, style);
}

/** 
 * \a p \a rect \a thread_index \a style 
 *
 * Here we just stitch the rectangle in the direction of it's longer side.
 */
void
embPattern_stitchRect(EmbPattern *p, EmbRect rect, int thread_index, int style)
{
    EmbReal seperation = 0.1;
    EmbReal width = rect.right - rect.left;
    EmbReal height = rect.bottom - rect.top;
    if (width > height) {
        float s;
        for (s=rect.top; s<rect.bottom; s += seperation) {
            /* Split long stitches here. */
            embPattern_addStitchAbs(p, rect.top, s, NORMAL, thread_index);
            embPattern_addStitchAbs(p, rect.bottom, s, NORMAL, thread_index);
        }
    }
    else {
        float s;
        for (s=rect.left; s<rect.right; s += seperation) {
            /* Split long stitches here. */
            embPattern_addStitchAbs(p, s, rect.left, NORMAL, thread_index);
            embPattern_addStitchAbs(p, s, rect.right, NORMAL, thread_index);
        }
    }
}

/**
 * \a p \a rect \a thread_index \a style 
 */
void
embPattern_stitchText(EmbPattern *p, EmbRect rect, int thread_index, int style)
{
    printf("DEBUG: %f %f %d %d",
        p->home.x, rect.top, thread_index, style);
}

/**
 * \a p 
 */
void
embPattern_convertGeometry(EmbPattern* p)
{
    int i;
    for (i=0; i<p->geometry->count; i++) {
        EmbGeometry g = p->geometry->geometry[i];
        switch (g.type) {
        case EMB_ARC: {
            /* To Do make the thread up here. */
            embPattern_stitchArc(p, g.object.arc, 0, 0);
            break;
        }
        case EMB_CIRCLE: {
            /* To Do make the thread up here. */
            embPattern_stitchCircle(p, g.object.circle, 0, 0);
            break;
        }
        case EMB_ELLIPSE: {
            /* To Do make the thread up here. */
            embPattern_stitchEllipse(p, g.object.ellipse, 0, 0);
            break;
        }
        case EMB_RECT: {
            /* To Do make the thread up here. */
            embPattern_stitchRect(p, g.object.rect, 0, 0);
            break;
        }
        default:
            break;
        }
    }
    /* Now ignore the geometry when writing. */
    p->geometry->count = 0;
}

