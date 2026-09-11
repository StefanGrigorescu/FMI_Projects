package com.eastwoodjtb.utils;


import org.slf4j.Marker;
import org.slf4j.MarkerFactory;

public final class LogMarkers {

    public static final Marker fatalMarker;


    static
    {
        fatalMarker = MarkerFactory.getMarker("FATAL");
    }

    private LogMarkers() {
        throw new java.lang.UnsupportedOperationException("Utility class and cannot be instantiated");
    }

}
