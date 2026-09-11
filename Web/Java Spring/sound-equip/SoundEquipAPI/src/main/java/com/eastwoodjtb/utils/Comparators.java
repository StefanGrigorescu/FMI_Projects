package com.eastwoodjtb.utils;

import com.eastwoodjtb.models.dto.ProductJoinedDTO;

import java.util.Comparator;


public final class Comparators {
    public static final Comparator<ProductJoinedDTO> byName;
    public static final Comparator<ProductJoinedDTO> bySubsidiaryName;
    public static final Comparator<ProductJoinedDTO> bySubsidiaryCountryCode;
    public static final Comparator<ProductJoinedDTO> byQuantity;


    static
    {
        byName = Comparator.comparing(ProductJoinedDTO::getName);

        bySubsidiaryName = Comparator.comparing(ProductJoinedDTO::getSubsidiaryName);

        bySubsidiaryCountryCode = Comparator.comparing(ProductJoinedDTO::getSubsidiaryCountryCode);

        byQuantity = Comparator.comparingInt(ProductJoinedDTO::getQuantity);

    }


    private Comparators() {
        throw new java.lang.UnsupportedOperationException("Utility class and cannot be instantiated");
    }

}
