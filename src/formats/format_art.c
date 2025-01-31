/*
 * ART FORMAT
 * This file is part of libembroidery.
 *
 * Copyright 2018-2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#include <stdio.h>
#include <math.h>

#include "../embroidery_internal.h"

/**
 * \file format_art.c The Bernina Embroidery Format (.art)
 * \addindex art
 * \addindex Bernina
 *
 * We don't know much about this format. \todo Find a source.
 */

char
readArt(EmbPattern* pattern, FILE* file)
{
    puts("readArt is not implemented");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish readArt */
}

char
writeArt(EmbPattern* pattern, FILE* file)
{
    puts("writeArt is not implemented");
    if (emb_verbose > 1) {
        printf("Called with %p %p\n", (void*)pattern, (void*)file);
    }
    return 0; /*TODO: finish writeArt */
}
