package com.example.notebook.notebooks

import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.notebook.data.INotebooksRepo
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class NotebookLockViewModel(
    savedStateHandle: SavedStateHandle,
    private val repo: INotebooksRepo,
): ViewModel() {
    private val _screenState = MutableStateFlow(NotebookLockScreenState())
    val screenState = _screenState.stateIn(
        viewModelScope,
        SharingStarted.WhileSubscribed(stopTimeoutMillis = 5000),
        NotebookLockScreenState()
    )

    private val notebookId: Long = checkNotNull(savedStateHandle["notebookId"]).toString().toLong()
    var notebookTitle: String? = null

    init {
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                notebookTitle = repo.getNotebookEntityById(notebookId)?.title
            }
        }
    }

    fun onEvent(event: NotebooksEvent): Unit {
        when(event) {
            is NotebooksEvent.SubmitNotebookLockFormEvent -> {
                val password = screenState.value.password
                if(password.isEmpty()) {
                    return
                }

                _screenState.update { it.copy(isLoading = true) }

                viewModelScope.launch {
                    val isPasswordCorrect = withContext(Dispatchers.IO) {
                        repo . checkPassword (notebookId, password)
                    }

                    _screenState.update { it.copy(
                        isPasswordCorrect = isPasswordCorrect,
                        isSubmitted = true,
                        isLoading = false,
                    ) }
                }
            }
            NotebooksEvent.ClearFormEvent -> {
                _screenState.update { it.copy(
                    password = "",
                    isPasswordCorrect = false,
                    isSubmitted = false,
                ) }
            }
            is NotebooksEvent.SetPasswordEvent -> {
                _screenState.update { it.copy(
                    password = event.password
                )  }
            }
            is NotebooksEvent.SetIsLoadingEvent -> {
                _screenState.update { it.copy(
                    isLoading = event.isLoading
                ) }
            }
            else -> {
                // Do nothing for other events
            }
        }
    }
}

data class NotebookLockScreenState(
    var password: String = "",
    var isPasswordCorrect: Boolean = false,
    var isSubmitted: Boolean = false,
    var isLoading: Boolean = false,
)
