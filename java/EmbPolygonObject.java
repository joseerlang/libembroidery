/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class EmbPolygonObject {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected EmbPolygonObject(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EmbPolygonObject obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libembroideryJNI.delete_EmbPolygonObject(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setPointList(EmbArray_ value) {
    libembroideryJNI.EmbPolygonObject_pointList_set(swigCPtr, this, EmbArray_.getCPtr(value), value);
  }

  public EmbArray_ getPointList() {
    long cPtr = libembroideryJNI.EmbPolygonObject_pointList_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbArray_(cPtr, false);
  }

  public void setLineType(int value) {
    libembroideryJNI.EmbPolygonObject_lineType_set(swigCPtr, this, value);
  }

  public int getLineType() {
    return libembroideryJNI.EmbPolygonObject_lineType_get(swigCPtr, this);
  }

  public void setColor(EmbColor value) {
    libembroideryJNI.EmbPolygonObject_color_set(swigCPtr, this, EmbColor.getCPtr(value), value);
  }

  public EmbColor getColor() {
    long cPtr = libembroideryJNI.EmbPolygonObject_color_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbColor(cPtr, false);
  }

  public EmbPolygonObject() {
    this(libembroideryJNI.new_EmbPolygonObject(), true);
  }

}
