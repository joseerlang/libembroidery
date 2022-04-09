/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class EmbArcObject {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected EmbArcObject(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EmbArcObject obj) {
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
        libembroideryJNI.delete_EmbArcObject(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setArc(EmbArc value) {
    libembroideryJNI.EmbArcObject_arc_set(swigCPtr, this, EmbArc.getCPtr(value), value);
  }

  public EmbArc getArc() {
    long cPtr = libembroideryJNI.EmbArcObject_arc_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbArc(cPtr, false);
  }

  public void setLineType(int value) {
    libembroideryJNI.EmbArcObject_lineType_set(swigCPtr, this, value);
  }

  public int getLineType() {
    return libembroideryJNI.EmbArcObject_lineType_get(swigCPtr, this);
  }

  public void setColor(EmbColor value) {
    libembroideryJNI.EmbArcObject_color_set(swigCPtr, this, EmbColor.getCPtr(value), value);
  }

  public EmbColor getColor() {
    long cPtr = libembroideryJNI.EmbArcObject_color_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbColor(cPtr, false);
  }

  public EmbArcObject() {
    this(libembroideryJNI.new_EmbArcObject(), true);
  }

}
