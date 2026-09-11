package com.eastwoodjtb.models.dto;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.RequiredArgsConstructor;


@Data
@EqualsAndHashCode
@RequiredArgsConstructor
public class RegisterDTO {
    private String name;

    private String password;

    private String ftpUsername;

    private String ftpPassword;

    private boolean ftpLogoutState;

}
