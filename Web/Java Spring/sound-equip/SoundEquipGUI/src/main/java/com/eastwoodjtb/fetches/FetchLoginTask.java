package com.eastwoodjtb.fetches;

import com.eastwoodjtb.utils.FetchUtil;
import com.google.gson.Gson;
import javafx.concurrent.Task;
import com.eastwoodjtb.dto.TokensDTO;

import java.util.HashMap;


public class FetchLoginTask extends Task<TokensDTO> {
    private static final String url = "http://localhost:8083/auth/login";
    private final String name;

    private final String password;


    public FetchLoginTask(String name, String password) {
        this.name = name;
        this.password = password;
    }

    @Override
    protected TokensDTO call() throws Exception {
        var values = new HashMap<String, String>()
        {{
            put("name", name);
            put("password", password);
        }};

        return FetchUtil.post(url, values, FetchLoginTask::parse);
    }

    private static TokensDTO parse(String jsonObjectBody)
    {
        Gson gson = new Gson();
        return gson.fromJson(jsonObjectBody, TokensDTO.class);
    }

}
