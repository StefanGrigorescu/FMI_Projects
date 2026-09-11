package com.example.notebook.api

import retrofit2.http.GET

interface SampleAPI {
    @GET("my/endpoint")
    fun getSample(): String {
        return "Successfully called sample the api"
    }
}
