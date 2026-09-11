package com.eastwoodjtb.security;


import lombok.Getter;
import lombok.RequiredArgsConstructor;


@RequiredArgsConstructor
@Getter
public enum RoleEnum {
    Admin(0, "Admin"),
    Manager(1, "Manager"),
    BasicUser(2, "BasicUser");


    private final int idx;
    private final String name;


}
