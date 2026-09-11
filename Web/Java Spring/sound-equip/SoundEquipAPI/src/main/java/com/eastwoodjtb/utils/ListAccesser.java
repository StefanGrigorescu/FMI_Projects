package com.eastwoodjtb.utils;

import lombok.extern.slf4j.Slf4j;
import org.springframework.dao.DataIntegrityViolationException;

import java.util.List;


@Slf4j
public final class ListAccesser {
    public static <T> T getOnlyOrNull(List<T> list)
    {
        if(list.size() < 1)
        {
            log.warn("ListAccesser.getOnlyOrNull: No object was found.");
            return null;
        }
        if(list.size() > 1)
            throw new DataIntegrityViolationException("ListAccesser.getOnlyOrNull: More than one objects were found.");

        return list.get(0);

    }


    private ListAccesser() {
        throw new java.lang.UnsupportedOperationException("Utility class and cannot be instantiated");
    }

}
