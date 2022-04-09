/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class EmbLine {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected EmbLine(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EmbLine obj) {
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
        libembroideryJNI.delete_EmbLine(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setStart(EmbVector value) {
    libembroideryJNI.EmbLine_start_set(swigCPtr, this, EmbVector.getCPtr(value), value);
  }

  public EmbVector getStart() {
    long cPtr = libembroideryJNI.EmbLine_start_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbVector(cPtr, false);
  }

  public void setEnd(EmbVector value) {
    libembroideryJNI.EmbLine_end_set(swigCPtr, this, EmbVector.getCPtr(value), value);
  }

  public EmbVector getEnd() {
    long cPtr = libembroideryJNI.EmbLine_end_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EmbVector(cPtr, false);
  }

  public EmbLine() {
    this(libembroideryJNI.new_EmbLine(), true);
  }

}
