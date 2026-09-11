package com.eastwoodjtb.models.dto;


import com.eastwoodjtb.models.Model;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import lombok.RequiredArgsConstructor;

import java.util.List;


@Data
@EqualsAndHashCode(callSuper = false)
@RequiredArgsConstructor
public class SubsidiaryDTO implements Model {
    private Long id;
    private String name;
    private String countryCode;
    private List<Long> productIds;


    @Override
    public Long getId() {
        return id;
    }

    @Override
    public String getName() {
        return name;
    }

}
