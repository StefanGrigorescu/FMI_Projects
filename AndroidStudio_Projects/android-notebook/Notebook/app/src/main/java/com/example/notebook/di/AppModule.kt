package com.example.notebook.di

import com.example.notebook.api.SampleAPI
import com.example.notebook.data.*
import com.example.notebook.notebooks.CreateNotebookViewModel
import com.example.notebook.notebooks.NotebookListViewModel
import com.example.notebook.notebooks.NotebookLockViewModel
import com.example.notebook.notes.CreateNoteViewModel
import com.example.notebook.notes.NoteListViewModel
import org.koin.android.ext.koin.androidContext
import org.koin.androidx.viewmodel.dsl.viewModel
import org.koin.dsl.module
import retrofit2.Retrofit
import retrofit2.converter.moshi.MoshiConverterFactory

val appModule = module {
    single<SampleAPI> {
        Retrofit.Builder()
            .baseUrl("https://google.com")
            .addConverterFactory((MoshiConverterFactory.create()))
            .build()
            .create(SampleAPI::class.java)
    }
    single<AppDatabase> {
        AppDatabase.getInstace(androidContext())
    }

    // Obs: There is also 'factory' is equivalent to Transient - a new instance for every requesting object

    // Notebooks
    single<NotebooksDao> {
        val database = get<AppDatabase>()
        database.notebooksDao
    }
    single<INotebooksRepo> {
        NotebooksRepo(get(), get())
    }
    viewModel {
        NotebookListViewModel(get())
    }
    viewModel {
        CreateNotebookViewModel(get())
    }
    viewModel {
        NotebookLockViewModel(get(), get())
    }

    // Notes
    single<NotesDao> {
        val database = get<AppDatabase>()
        database.notesDao
    }
    single<INotesRepo> {
        NotesRepo(get())
    }
    viewModel {
        NoteListViewModel(get(), get())
    }
    viewModel {
        CreateNoteViewModel(get(), get())
    }
}
