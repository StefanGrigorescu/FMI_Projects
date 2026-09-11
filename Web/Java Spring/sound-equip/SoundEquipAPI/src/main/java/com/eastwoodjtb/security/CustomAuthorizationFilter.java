package com.eastwoodjtb.security;

import com.auth0.jwt.JWT;
import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.eastwoodjtb.services.SoundEquipUserService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.OncePerRequestFilter;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

import static java.util.Arrays.stream;
import static org.springframework.http.HttpHeaders.AUTHORIZATION;


@Slf4j
@RequiredArgsConstructor
public class CustomAuthorizationFilter extends OncePerRequestFilter {
    private final SoundEquipUserService soundEquipUserService;


    @Override
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {
        if(request.getServletPath().equals("/auth/register") || request.getServletPath().equals("/auth/refresh-token") || request.getServletPath().equals("/auth/login")) {
            filterChain.doFilter(request, response);    // if it's a request for login or register, we let it pass through
            return;
        }

        String authorizationHeader = request.getHeader(AUTHORIZATION);
        if(authorizationHeader == null || !authorizationHeader.startsWith("Bearer "))
        {
            filterChain.doFilter(request, response);
            return;
        }

        try {
            DecodedJWT decodedJWT = TokenUtil.getDecodedJWT(authorizationHeader);   // access token decoded

            String username = decodedJWT.getSubject();
            soundEquipUserService.checkAccessToken(username, authorizationHeader.substring("Bearer ".length()));

            String[] roles = decodedJWT.getClaim(AuthUtil.getRolesClaimName()).asArray(String.class);

            Collection<SimpleGrantedAuthority> authorities = new ArrayList<>();
            stream(roles).forEach(role -> {
                authorities.add(new SimpleGrantedAuthority(role));
            });

            UsernamePasswordAuthenticationToken authenticationToken =
                    new UsernamePasswordAuthenticationToken(username, null, authorities);

            SecurityContextHolder.getContext().setAuthentication(authenticationToken);
            filterChain.doFilter(request, response);
        }
        catch (Exception e) {
            log.error("CustomAuthorizationFilter.doFilterInternal: Application authorization header error: {}", e.getMessage());
            TokenUtil.setResponseError(response, e.getMessage());
        }
    }

}
