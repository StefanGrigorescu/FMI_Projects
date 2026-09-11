package com.eastwoodjtb.models.dto;

import lombok.Data;
import lombok.RequiredArgsConstructor;


@Data
@RequiredArgsConstructor
public class AddRoleDTO
{
    private String username;
    private String roleName;

}
