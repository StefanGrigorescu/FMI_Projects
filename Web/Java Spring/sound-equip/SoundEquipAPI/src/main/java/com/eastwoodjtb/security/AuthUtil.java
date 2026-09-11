package com.eastwoodjtb.security;


import com.auth0.jwt.algorithms.Algorithm;
import java.nio.charset.StandardCharsets;

public class AuthUtil {
    private static final Algorithm algorithm;
    private static final String rolesClaimName;
    private static final long accessTokenLifespan;
    private static final long refreshTokenLifespan;


    static {
        algorithm = Algorithm.HMAC256("justADummyAsSecretForNow".getBytes(StandardCharsets.UTF_8));
        rolesClaimName = "roles";
        accessTokenLifespan = 10 * 60 * 1000;               // 10 minutes valability
        refreshTokenLifespan = 30L * 24 * 60 * 60 * 1000;   // 30 days valability
    }

    public static Algorithm getAlgorithm()
    {
        return algorithm;
    }
    public static String getRolesClaimName() { return rolesClaimName; }
    public static long getAccessTokenLifespan() { return accessTokenLifespan; }
    public static long getRefreshTokenLifespan() { return refreshTokenLifespan; }

}
