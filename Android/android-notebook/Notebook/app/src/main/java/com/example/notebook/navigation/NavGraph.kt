package com.example.notebook.navigation

import androidx.compose.runtime.Composable
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import com.example.notebook.notes.NoteListScreen
import com.example.notebook.notebooks.CreateNotebookScreen
import com.example.notebook.notebooks.NotebookListScreen
import com.example.notebook.notebooks.NotebookLockScreen
import com.example.notebook.notes.CreateNoteScreen
import com.example.notebook.profile.ProfileScreen
import com.example.notebook.settings.SettingsScreen

@Composable
fun SetupNavGraph(
    navController: NavHostController
) {
    NavHost(
        navController = navController,
        startDestination = Screen.NotebookList.route
    ) {
        // Notebooks
        composable(Screen.NotebookList.route) { NotebookListScreen(navController = navController) }
        composable(Screen.CreateNotebook.route) { CreateNotebookScreen(navController = navController) }
        composable(Screen.NotebookLock.route) { backStackEntry ->
            val notebookId: Long? = backStackEntry.arguments?.getString("notebookId")?.toLongOrNull()
            requireNotNull(notebookId) { "NavigationError: notebookId parameter not found." }
            NotebookLockScreen(navController = navController, notebookId = notebookId)
        }
        // Notes
        composable(Screen.NoteList.route) { NoteListScreen(navController = navController) }
        composable(Screen.CreateNote.route) { CreateNoteScreen(navController = navController) }

        // Profile
        composable(BottomBarScreen.Profile.route) { ProfileScreen(navController = navController) }

        // Settings
        composable(BottomBarScreen.Settings.route) { SettingsScreen(navController = navController) }
    }
}
