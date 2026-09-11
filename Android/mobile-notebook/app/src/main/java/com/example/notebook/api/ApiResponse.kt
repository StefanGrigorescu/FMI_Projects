package com.example.notebook.api

class ApiResponse<T> {
    var data: T? = null
    var errorMessages: List<String> = listOf()
}
