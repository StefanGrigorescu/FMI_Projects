package com.eastwoodjtb.utils;


public enum StageTitleEnum {
    home("home"),
    login("login"),
    register("register");


    private final String title;


    StageTitleEnum(String title)
    {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }
}
