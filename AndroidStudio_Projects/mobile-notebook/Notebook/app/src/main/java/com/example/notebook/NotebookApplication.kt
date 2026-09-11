package com.example.notebook

import android.app.Application
import com.example.notebook.di.appModule
import org.koin.android.ext.koin.androidContext
import org.koin.android.ext.koin.androidLogger
import org.koin.core.context.startKoin

class NotebookApplication: Application() {
    override fun onCreate() {
        super.onCreate()
        startKoin {
            androidLogger()
            androidContext(this@NotebookApplication)
            modules(appModule)
        }
    }
}
