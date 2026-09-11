package com.example.notebook.notes

import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.notebook.data.INotesRepo
import com.example.notebook.data.NoteEntity
import com.example.notebook.notebooks.NotebooksEvent
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

class CreateNoteViewModel(
    savedStateHandle: SavedStateHandle,
    private val repo: INotesRepo
): ViewModel() {
    private val _screenState = MutableStateFlow(CreateNoteScreenState())
    val screenState = _screenState.stateIn(viewModelScope, SharingStarted.WhileSubscribed(stopTimeoutMillis = 5000), CreateNoteScreenState())

    private val notebookId: Long = checkNotNull(savedStateHandle["notebookId"]).toString().toLong()

    fun onEvent(event: NotesEvent): Unit {
        when (event) {
            is NotesEvent.SubmitCreateNoteFormEvent -> {
                val title = screenState.value.title

                if (title.isEmpty()) {
                    return
                }

                val noteEntity = NoteEntity(
                    title = title,
                    notebookId = notebookId
                )

                viewModelScope.launch {
                    repo.insert(noteEntity)
                }

                _screenState.update {
                    it.copy(
                        title = "",
                    )
                }
            }
            NotesEvent.ClearFormEvent -> {
                _screenState.update {
                    it.copy(
                        title = "",
                    )
                }
            }
            is NotesEvent.SetTitleEvent -> {
                _screenState.update {
                    it.copy(
                        title = event.title
                    )
                }
            }
            else -> {
                // Do nothing for other events
            }
        }
    }
}

data class CreateNoteScreenState(
    var title: String = "",
)
