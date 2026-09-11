package com.eastwoodjtb.fetches;

import com.eastwoodjtb.utils.FetchUtil;
import com.google.gson.Gson;
import javafx.concurrent.Task;

import java.util.HashMap;


public class FetchRegisterTask extends Task<Boolean> {
    private static final String url = "http://localhost:8083/auth/register";
    private final String name;

    private final String password;

    private final String ftpUsername;

    private final String ftpPassword;


    public FetchRegisterTask(String name, String password, String ftpUsername, String ftpPassword) {
        this.name = name;
        this.password = password;
        this.ftpUsername = ftpUsername;
        this.ftpPassword = ftpPassword;
    }

    @Override
    protected Boolean call() throws Exception {
        var values = new HashMap<String, String>()
        {{
            put("name", name);
            put("password", password);
            put("ftpUsername", ftpUsername);
            put("ftpPassword", ftpPassword);
        }};

        return FetchUtil.post(url, values, FetchRegisterTask::parse);
    }

/*    public static String parse(String responseBody)
    {
        JsonArray jsonArray = new JsonArray(responseBody);
        for(int i = 0; i < jsonArray.size(); i++)
            JsonObject jsonObject = jsonArray.getAsJsonObject(i);

        return null;
    }*/

    private static Boolean parse(String jsonObjectBody)
    {
        Gson gson = new Gson();
        return gson.fromJson(jsonObjectBody, Boolean.class);
    }

}
