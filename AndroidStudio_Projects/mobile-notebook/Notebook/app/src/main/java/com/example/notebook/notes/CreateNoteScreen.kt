package com.example.notebook.notes

import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.OutlinedTextField
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar
import com.example.notebook.navigation.Screen
import com.example.notebook.notes.*
import org.koin.androidx.compose.getViewModel

@Composable
fun CreateNoteScreen(
    navController: NavHostController
) {
    val viewModel: CreateNoteViewModel = getViewModel<CreateNoteViewModel>()
    val state: CreateNoteScreenState by viewModel.screenState.collectAsState()
    
    Scaffold(
        topBar = {
            AppBrand(Screen.CreateNote.screenName)
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
                NoteTitleInput(state.title) { event ->
                    viewModel.onEvent(event)
                }

                // Submit, Clear, and Cancel buttons
                Row(
                    horizontalArrangement = Arrangement.spacedBy(8.dp),
                    modifier = Modifier.fillMaxWidth()
                ) {
                    Button(
                        onClick = {
                            viewModel.onEvent(
                                NotesEvent.SubmitCreateNoteFormEvent(
                                state.title,
                            ))
                            navController.popBackStack()
                        },
                        modifier = Modifier.weight(1f)
                    ) {
                        Text("Submit")
                    }

                    Button(
                        onClick = {
                            viewModel.onEvent(NotesEvent.ClearFormEvent)
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
fun NoteTitleInput(
    title: String,
    onSetTitleEvent: (NotesEvent.SetTitleEvent) -> Unit
) {
    OutlinedTextField(
        value = title,
        onValueChange = { newValue: String ->
            onSetTitleEvent(NotesEvent.SetTitleEvent(newValue))
        },
        label = { Text("Note Name") },
        singleLine = true,
        modifier = Modifier.fillMaxWidth()
    )
}
