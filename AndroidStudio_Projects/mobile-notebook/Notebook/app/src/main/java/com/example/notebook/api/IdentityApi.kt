package com.example.notebook.api

import retrofit2.http.Body
import retrofit2.http.POST
import java.time.Instant

object IdentityApiSpecification {
    const val name: String = "DMExtendedAPIJava"

    object Routes {
        const val root: String = "http://localhost:3000/api/v1";
    }
}


interface IdentityApi {
    @POST("/identity/login/jwt/username")
    suspend fun loginWithUsername(
        @Body request: LoginWithUsernameRequest
    ): ApiResponse<JwtLoginResponse>
}


class LoginWithUsernameRequest {
    val username: String? = null
    val password: String? = null
}


class JwtLoginResponse {
    val accessToken: AccessJwtResponse? = null
    val refreshToken: RefreshTokenResponse? = null
    val id: String? = null
    val username: String? = null
    val email: String? = null
    val roles: List<String>? = null
}


class AccessJwtResponse {
    val jwt: String? = null
    val expireTime: Instant? = null
}


class RefreshTokenResponse {
    var token: String? = null
    var expireDate: Instant? = null
}
