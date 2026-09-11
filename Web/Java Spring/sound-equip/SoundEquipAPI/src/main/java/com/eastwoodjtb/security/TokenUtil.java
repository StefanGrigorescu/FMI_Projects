package com.eastwoodjtb.security;

import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.fasterxml.jackson.databind.ObjectMapper;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import static org.springframework.http.HttpStatus.FORBIDDEN;
import static org.springframework.http.MediaType.APPLICATION_JSON_VALUE;


public class TokenUtil {
    public static void setResponseError(HttpServletResponse response, String errorMessage) throws IOException {
        response.setHeader("error", errorMessage);
        // response.sendError(FORBIDDEN.value());
        response.setStatus(FORBIDDEN.value());
        Map<String, String> error = new HashMap<>();
        error.put("error_message", errorMessage);
        response.setContentType(APPLICATION_JSON_VALUE);
        new ObjectMapper().writeValue(response.getOutputStream(), error);
    }

    public static void setResponseTokens(HttpServletResponse response, String accessToken, String refreshToken) throws IOException {
        Map<String, String> tokens = new HashMap<>();
        tokens.put("access_token", accessToken);
        tokens.put("refresh_token", refreshToken);
        response.setContentType(APPLICATION_JSON_VALUE);
        new ObjectMapper().writeValue(response.getOutputStream(), tokens);
    }

    public static DecodedJWT getDecodedJWT (String authorizationHeader)
    {
        String token = authorizationHeader.substring("Bearer ".length());   // we remove the first letters of the string to extract the token
        Algorithm algorithm = AuthUtil.getAlgorithm();

        JWTVerifier verifier = JWT.require(algorithm).build();

        return verifier.verify(token);
    }

}
