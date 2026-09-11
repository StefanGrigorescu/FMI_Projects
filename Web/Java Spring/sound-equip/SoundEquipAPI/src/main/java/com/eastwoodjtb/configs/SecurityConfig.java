package com.eastwoodjtb.configs;

import com.eastwoodjtb.security.CustomAuthenticationFilter;
import com.eastwoodjtb.security.CustomAuthorizationFilter;
import com.eastwoodjtb.security.RoleEnum;
import com.eastwoodjtb.services.SoundEquipUserService;
import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

import static org.springframework.http.HttpMethod.*;
import static org.springframework.security.config.http.SessionCreationPolicy.STATELESS;


@Configuration
@EnableWebSecurity
@RequiredArgsConstructor
public class SecurityConfig extends WebSecurityConfigurerAdapter {
    private final UserDetailsService userDetailsService;
    private final BCryptPasswordEncoder bCryptPasswordEncoder;
    private final SoundEquipUserService soundEquipUserService;


    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth.userDetailsService(userDetailsService).passwordEncoder(bCryptPasswordEncoder);
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        CustomAuthenticationFilter customAuthenticationFilter = new CustomAuthenticationFilter(authenticationManagerBean(), soundEquipUserService);
        customAuthenticationFilter.setFilterProcessesUrl("/auth/login");
        http.csrf().disable();
        http.sessionManagement().sessionCreationPolicy(STATELESS);
        http.authorizeRequests().antMatchers("/auth/register/**", "/auth/login/**", "/auth/refresh-token/**", "/swagger-ui.html").permitAll();

        http.authorizeRequests().antMatchers(GET,  "subsidiary/**", "/product/**").hasAnyAuthority(RoleEnum.BasicUser.getName());
        http.authorizeRequests().antMatchers(GET,  "subsidiary/**", "/product/**").hasAnyAuthority(RoleEnum.Manager.getName());

        authorizeAllRequestsAtPath(http, "/subsidiary/**", RoleEnum.Admin.getName());
        authorizeAllRequestsAtPath(http, "/product/**", RoleEnum.Admin.getName());
        authorizeAllRequestsAtPath(http, "/sound-equip-user/**", RoleEnum.Admin.getName());

        http.authorizeRequests().anyRequest().authenticated();

        http.addFilter(customAuthenticationFilter);
        http.addFilterBefore(new CustomAuthorizationFilter(soundEquipUserService), UsernamePasswordAuthenticationFilter.class);
    }

    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();  // the authenticationManager from this' super class, namely, WebSecurityConfigurerAdapter
    }

    private void authorizeAllRequestsAtPath(HttpSecurity http, String antPatterns, String authority) throws Exception
    {
        http.authorizeRequests().antMatchers(GET, antPatterns).hasAnyAuthority(authority);
        http.authorizeRequests().antMatchers(POST, antPatterns).hasAnyAuthority(authority);
        http.authorizeRequests().antMatchers(PUT, antPatterns).hasAnyAuthority(authority);
        http.authorizeRequests().antMatchers(DELETE, antPatterns).hasAnyAuthority(authority);
    }

}
