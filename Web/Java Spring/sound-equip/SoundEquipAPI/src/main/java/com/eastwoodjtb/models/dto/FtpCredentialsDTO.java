package com.eastwoodjtb.models.dto;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.RequiredArgsConstructor;


@Data
@EqualsAndHashCode
@RequiredArgsConstructor
public class FtpCredentialsDTO {

    private String name;

    private String ftpUsername;

    private String ftpPassword;

}
