package com.example.notebook.notebooks

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.notebook.data.INotebooksRepo
import com.example.notebook.data.NotebookEntity
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch

class CreateNotebookViewModel(
    private val repo: INotebooksRepo
): ViewModel() {
    private val _screenState = MutableStateFlow(CreateNotebookScreenState())
    val screenState = _screenState.stateIn(viewModelScope, SharingStarted.WhileSubscribed(stopTimeoutMillis = 5000), CreateNotebookScreenState())

    fun onEvent(event: NotebooksEvent): Unit {
        when(event) {
            is NotebooksEvent.SubmitCreateNotebookFormEvent -> {
                val title = screenState.value.title
                val description = screenState.value.description
                val password = screenState.value.password
                val passwordConfirm = screenState.value.confirmPassword

                if(title.isEmpty()) {
                    return
                }
                if(password.isEmpty() != passwordConfirm.isEmpty()) {
                    return
                }
                if(password.isNotEmpty() && password != passwordConfirm) {
                    return
                }

                val notebookEntity = NotebookEntity(
                    title = title,
                    description = description,
                    password = password
                )

                viewModelScope.launch {
                    repo.insert(notebookEntity)
                }

                _screenState.update { it.copy(
                    title = "",
                    description = "",
                    password = "",
                    confirmPassword = "",
                ) }
            }
            NotebooksEvent.ClearFormEvent -> {
                _screenState.update { it.copy(
                    title = "",
                    description = "",
                    password = "",
                    confirmPassword = ""
                ) }
            }
            is NotebooksEvent.SetTitleEvent -> {
                _screenState.update { it.copy(
                    title = event.title
                ) }
            }
            is NotebooksEvent.SetDescriptionEvent -> {
                _screenState.update { it.copy(
                    description = event.description
                ) }
            }
            is NotebooksEvent.SetPasswordEvent -> {
                _screenState.update { it.copy(
                    password = event.password
                ) }

                if(event.password.isEmpty()) {
                    _screenState.update { it.copy(
                        confirmPassword = ""
                    ) }
                }
            }
            is NotebooksEvent.SetConfirmPasswordEvent -> {
                _screenState.update { it.copy(
                    confirmPassword = event.confirmPassword
                ) }
            }
            else -> {
                // Do nothing for other events
            }
        }
    }
}

data class CreateNotebookScreenState(
    var title: String = "",
    var description: String = "",
    var password: String = "",
    var confirmPassword: String = "",
)
