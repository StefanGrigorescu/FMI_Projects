package com.eastwoodjtb.models.dto;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.RequiredArgsConstructor;


@Data
@EqualsAndHashCode
@RequiredArgsConstructor
public class FtpStateDTO {
    private String name;
    private boolean ftpLogoutState;


    public boolean getFtpLogoutState() {
        return ftpLogoutState;
    }

}
