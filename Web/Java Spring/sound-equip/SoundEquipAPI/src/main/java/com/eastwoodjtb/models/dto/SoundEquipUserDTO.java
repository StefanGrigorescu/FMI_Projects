package com.eastwoodjtb.models.dto;

import com.eastwoodjtb.models.entities.Role;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.RequiredArgsConstructor;

import java.util.ArrayList;
import java.util.Collection;


@Data
@EqualsAndHashCode
@RequiredArgsConstructor
public class SoundEquipUserDTO {
    private Long id;
    private String name;
    private String ftpUsername;
    private boolean ftpLogoutState;
    private Collection<Role> roles = new ArrayList<>();

}
