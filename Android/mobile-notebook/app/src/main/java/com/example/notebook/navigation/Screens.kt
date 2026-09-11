package com.example.notebook.navigation

import androidx.compose.foundation.layout.RowScope
import androidx.compose.material.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Person
import androidx.compose.material.icons.filled.Settings
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.navigation.NavDestination
import androidx.navigation.NavDestination.Companion.hierarchy
import androidx.navigation.NavGraph.Companion.findStartDestination
import androidx.navigation.NavHostController
import androidx.navigation.compose.currentBackStackEntryAsState

sealed class Screen(val route: String, val screenName: String) {
    // Notebooks
    object NotebookList : Screen(
        route = "notebooks/list",
        screenName = "Notebooks List"
    )
    object CreateNotebook : Screen(
        route = "notebooks/create",
        screenName = "Create Notebook"
    )
    object NotebookLock: Screen(
        route = "notebooks/{notebookId}/locked",
        screenName = "Notebook Lock"
    ) {
        fun routeFactory(notebookId: Long?) = "notebooks/$notebookId/locked"
    }

    // Notes
    object NoteList : Screen(
        route = "notebooks/{notebookId}/notes/list",
        screenName = "Notes List"
    ) {
        fun routeFactory(notebookId: Long?) = "notebooks/$notebookId/notes/list"
    }
    object CreateNote : Screen(
        route = "notebooks/{notebookId}/notes/create",
        screenName = "Create Notebook"
    ) {
        fun routeFactory(notebookId: Long?) = "notebooks/$notebookId/notes/create"
    }
}

sealed class BottomBarScreen (
    val route: String,
    val screenName: String,
    val icon: ImageVector
) {
    object NotebookList: BottomBarScreen(
        route = "notebooks/list",
        screenName = "Notebooks List",
        icon = Icons.Default.Home
    )
    object Profile: BottomBarScreen(
        route = "profile",
        screenName = "Profile",
        icon = Icons.Default.Person
    )
    object Settings: BottomBarScreen(
        route = "settings",
        screenName = "Settings",
        icon = Icons.Default.Settings
    )
}

@Composable
fun BottomBar(navController: NavHostController) {
    val screens = listOf(
        BottomBarScreen.NotebookList,
        BottomBarScreen.Profile,
        BottomBarScreen.Settings
    )
    val navBackStackEntry by navController.currentBackStackEntryAsState()
    val currentDestination = navBackStackEntry?.destination

    BottomNavigation() {
        screens.forEach { screen ->
            AddItem(
                screen = screen,
                currentDestination = currentDestination,
                navController = navController
            )
        }
    }
}

@Composable
fun RowScope.AddItem(
    screen: BottomBarScreen,
    currentDestination: NavDestination?,
    navController: NavHostController
) {
    BottomNavigationItem(
        label = {
                Text(text = screen.screenName)
        },
        icon = {
            Icon(
                imageVector = screen.icon,
                contentDescription = "Navigation Icon"
            )
        },
        selected =  currentDestination?.hierarchy?.any {
            it.route == screen.route
        } == true,
        onClick = {
            navController.navigate(
                screen.route
            ) {
                popUpTo(navController.graph.findStartDestination().id)
                launchSingleTop = true
            }
        },
        unselectedContentColor = LocalContentColor.current.copy(alpha = ContentAlpha.disabled)
    )
}
