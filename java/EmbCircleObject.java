/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class EmbCircleObject {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected EmbCircleObject(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EmbCircleObject obj) {
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
        libembroideryJNI.delete_EmbCircleObject(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setCircle(EmbCircle value) {
    libembroideryJNI.EmbCircleObject_circle_set(swigCPtr, this, EmbCircle.getCPtr(value), value);
  }

  public EmbCircle getCircle() {
    long cPtr = libembroideryJNI.EmbCircleObject_circle_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbCircle(cPtr, false);
  }

  public void setLineType(int value) {
    libembroideryJNI.EmbCircleObject_lineType_set(swigCPtr, this, value);
  }

  public int getLineType() {
    return libembroideryJNI.EmbCircleObject_lineType_get(swigCPtr, this);
  }

  public void setColor(EmbColor value) {
    libembroideryJNI.EmbCircleObject_color_set(swigCPtr, this, EmbColor.getCPtr(value), value);
  }

  public EmbColor getColor() {
    long cPtr = libembroideryJNI.EmbCircleObject_color_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbColor(cPtr, false);
  }

  public void setCount(int value) {
    libembroideryJNI.EmbCircleObject_count_set(swigCPtr, this, value);
  }

  public int getCount() {
    return libembroideryJNI.EmbCircleObject_count_get(swigCPtr, this);
  }

  public void setLength(int value) {
    libembroideryJNI.EmbCircleObject_length_set(swigCPtr, this, value);
  }

  public int getLength() {
    return libembroideryJNI.EmbCircleObject_length_get(swigCPtr, this);
  }

  public EmbCircleObject() {
    this(libembroideryJNI.new_EmbCircleObject(), true);
  }

}
