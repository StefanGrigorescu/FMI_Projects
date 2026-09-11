package com.example.notebook.settings

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar

@Composable
fun SettingsScreen(navController: NavHostController) {
    Scaffold(
        topBar = {
            AppBrand("Settings")
        },
        bottomBar = { BottomBar(navController = navController) },
        content = { padding ->
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(padding),
//            contentAlignment = Alignment.Center
            ) {
                Text(
                    text = "Settings",
                    fontSize = MaterialTheme.typography.h6.fontSize,
                    modifier = Modifier.padding(PaddingValues(start = 20.dp, top=8.dp))
                )
            }
        }
    )
}
