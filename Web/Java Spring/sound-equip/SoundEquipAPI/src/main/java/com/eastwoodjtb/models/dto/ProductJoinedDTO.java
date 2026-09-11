package com.eastwoodjtb.models.dto;

import com.eastwoodjtb.models.Model;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import lombok.RequiredArgsConstructor;

import java.util.Date;


@Data
@EqualsAndHashCode(callSuper = false)
@RequiredArgsConstructor
public class ProductJoinedDTO implements Model {
    private Long id;
    private String name;
    private String status;
    private int quantity;
    private Date stockDeclDate;
    private Long subsidiaryId;
    private String subsidiaryName;
    private String subsidiaryCountryCode;


    @Override
    public Long getId() {
        return id;
    }

    @Override
    public String getName() {
        return name;
    }

}
