package org.fuzzyc;

public class Libfuzzyc
{
	static
	{
		System.loadLibrary("gl2jni");
	}
	
	public static native void init(int width, int height);
  public static native void step();
}
