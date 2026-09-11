package com.eastwoodjtb.utils;

import com.eastwoodjtb.models.Model;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.ArrayList;
import java.util.List;

public final class ConverterUtil {

    public static <TEntity extends Model> List<TEntity> toEntityList(List<Long> idList, JpaRepository<TEntity, Long> entityRepository)
    {
        List<TEntity> entityList = new ArrayList<>();
        idList.forEach(id ->
                entityList
                        .add(entityRepository.getById(id)));

        return entityList;
    }

    public static <TEntity extends Model> List<Long> toIdList(List<TEntity> entities)
    {
        List<Long> idList = new ArrayList<>();
        entities.forEach(entity ->
                idList
                        .add(entity.getId()));

        return idList;
    }


    private ConverterUtil() {
        throw new java.lang.UnsupportedOperationException("Utility class and cannot be instantiated");
    }

}
