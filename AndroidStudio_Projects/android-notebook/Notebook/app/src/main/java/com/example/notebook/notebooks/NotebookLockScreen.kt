package com.example.notebook.notebooks

import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar
import com.example.notebook.navigation.Screen
import org.koin.androidx.compose.getViewModel

@Composable
fun NotebookLockScreen(
    notebookId: Long?,
    navController: NavHostController
) {
    val viewModel: NotebookLockViewModel = getViewModel<NotebookLockViewModel>()
    val state: NotebookLockScreenState by viewModel.screenState.collectAsState()

    Scaffold(
        topBar = {
            AppBrand(Screen.NotebookLock.screenName)
        },
        bottomBar = { BottomBar(navController = navController) }
    ) { padding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
                .padding(PaddingValues(start = 20.dp, end = 20.dp, top = 10.dp)),
            verticalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            val notebookTitle = viewModel.notebookTitle
            val notebookLockedText = if(notebookTitle.isNullOrBlank())
                    "This notebook is locked!"
                else
                    "The notebook '$notebookTitle' is locked!"
            Text(notebookLockedText)

            NotebookPasswordInput(
                state.password
            ) { event -> viewModel.onEvent(event) }
            if(state.isSubmitted && !state.isPasswordCorrect) {
                Text(
                    "Incorrect password. Notebook is still locked out.",
                    color = Color.Red)
            }

            // This code will be executed only after the layout pass is complete
            LaunchedEffect(state) {
                // Listen to state.isPasswordCorrect and navigate when it is set to true
                if (state.isPasswordCorrect) {
                    navController.popBackStack()
                    navController.navigate(Screen.NoteList.routeFactory(notebookId))
                }
            }

            // Submit, Clear, and Cancel buttons
            Row(
                horizontalArrangement = Arrangement.spacedBy(8.dp),
                modifier = Modifier.fillMaxWidth()
            ) {
                Button(
                    onClick = {
                        viewModel.onEvent(NotebooksEvent.SubmitNotebookLockFormEvent(state.password))
                    },
                    modifier = Modifier.weight(1f),
                    enabled = !state.isLoading,
                ) {
                    Text("Submit")
                }

                Button(
                    onClick = {
                        viewModel.onEvent(NotebooksEvent.ClearFormEvent)
                    },
                    modifier = Modifier.weight(1f),
                    enabled = !state.isLoading,
                ) {
                    Text("Clear")
                }

                Button(
                    onClick = {
                        navController.popBackStack()
                    },
                    modifier = Modifier.weight(1f),
                    enabled = !state.isLoading,
                ) {
                    Text("Cancel")
                }
            }
        }
    }
}
