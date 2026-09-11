package com.example.notebook.settings

import android.content.Context
import android.content.SharedPreferences

class Preferences(context: Context) {
    private val sharedPreferences: SharedPreferences = context.getSharedPreferences(
        "Preferences",
        Context.MODE_PRIVATE
    )

    fun saveNew(key: String, value: String) {
        val editor = sharedPreferences.edit()
        editor.putString(key, value)
        editor.apply()
    }

    fun check(key: String) : String? {
        return sharedPreferences.getString(key, null)
    }
}
