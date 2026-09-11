package com.example.notebook.notebooks

import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.OutlinedTextField
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.input.PasswordVisualTransformation
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar
import com.example.notebook.navigation.Screen
import org.koin.androidx.compose.getViewModel

@Composable
fun CreateNotebookScreen(
    navController: NavHostController
) {
    val viewModel: CreateNotebookViewModel = getViewModel<CreateNotebookViewModel>()
    val state: CreateNotebookScreenState by viewModel.screenState.collectAsState()

    Scaffold(
        topBar = {
            AppBrand(Screen.CreateNotebook.screenName)
        },
        bottomBar = { BottomBar(navController = navController) },
        content = { padding ->
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(padding)
                    .padding(PaddingValues(start = 20.dp, end = 20.dp,)),
                verticalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                NotebookTitleInput(state.title) { event ->
                    viewModel.onEvent(event)
                }

                NotebookDescriptionInput(state.description) { event ->
                    viewModel.onEvent(event)
                }

                NotebookPasswordInput(state.password, addOptionalToPlaceholder = true) { event ->
                    viewModel.onEvent(event)
                }

                if (state.password.isNotBlank()) {
                    NotebookPasswordConfirmationInput(state.password, state.confirmPassword) { event ->
                        viewModel.onEvent(event)
                    }
                }

                // Submit, Clear, and Cancel buttons
                Row(
                    horizontalArrangement = Arrangement.spacedBy(8.dp),
                    modifier = Modifier.fillMaxWidth()
                ) {
                    Button(
                        onClick = {
                            viewModel.onEvent(NotebooksEvent.SubmitCreateNotebookFormEvent(
                                state.title,
                                state.description,
                                state.password
                            ))
                            navController.popBackStack()
                        },
                        modifier = Modifier.weight(1f)
                    ) {
                        Text("Submit")
                    }

                    Button(
                        onClick = {
                            viewModel.onEvent(NotebooksEvent.ClearFormEvent)
                        },
                        modifier = Modifier.weight(1f)
                    ) {
                        Text("Clear")
                    }

                    Button(
                        onClick = {
                            navController.popBackStack()
                        },
                        modifier = Modifier.weight(1f)
                    ) {
                        Text("Cancel")
                    }
                }
            }
        }
    )
}

@Composable
fun NotebookTitleInput(
    title: String,
    onSetTitleEvent: (NotebooksEvent.SetTitleEvent) -> Unit
) {
    OutlinedTextField(
        value = title,
        onValueChange = { newValue: String ->
            onSetTitleEvent(NotebooksEvent.SetTitleEvent(newValue))
        },
        label = { Text("Notebook Name") },
        singleLine = true,
        modifier = Modifier.fillMaxWidth()
    )
}

@Composable
fun NotebookDescriptionInput(
    description: String,
    onSetDescriptionEvent: (NotebooksEvent.SetDescriptionEvent) -> Unit
) {
    OutlinedTextField(
        value = description,
        onValueChange = { newValue: String ->
            onSetDescriptionEvent(NotebooksEvent.SetDescriptionEvent(newValue))
        },
        label = { Text("Notebook Description") },
        modifier = Modifier.fillMaxWidth()
    )
}

@Composable
fun NotebookPasswordInput(
    password: String,
    addOptionalToPlaceholder: Boolean = false,
    onSetPasswordEvent: (NotebooksEvent.SetPasswordEvent) -> Unit,
) {
    OutlinedTextField(
        value = password,
        onValueChange = { newValue: String ->
            onSetPasswordEvent(NotebooksEvent.SetPasswordEvent(newValue))
        },
        label = {
            val placeholder: String = if(addOptionalToPlaceholder) "Notebook Password (Optional)" else "Notebook Password"
            Text(placeholder)
        },
        singleLine = true,
        visualTransformation = PasswordVisualTransformation(),
        modifier = Modifier.fillMaxWidth()
    )
}

@Composable
fun NotebookPasswordConfirmationInput(
    confirmPassword: String, notebookPasswordConfirm: String,
    onSetConfirmPasswordEvent: (NotebooksEvent.SetConfirmPasswordEvent) -> Unit
) {
    if (confirmPassword.isNotBlank()) {
        OutlinedTextField(
            value = notebookPasswordConfirm,
            onValueChange = { newValue: String ->
                onSetConfirmPasswordEvent(NotebooksEvent.SetConfirmPasswordEvent(newValue))
            },
            label = { Text("Confirm Password") },
            singleLine = true,
            visualTransformation = PasswordVisualTransformation(),
            modifier = Modifier.fillMaxWidth()
        )
    }
}
