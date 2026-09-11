package com.example.notebook.settings

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.material.MaterialTheme
import androidx.compose.material.RadioButton
import androidx.compose.material.Scaffold
import androidx.compose.material.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar

@Composable
fun SettingsScreen(navController: NavHostController) {
    val context = LocalContext.current
    val preferences = remember { Preferences(context) }

    // Load the saved preference
    var savedSortingOption by remember { mutableStateOf(
        preferences.check("Notebooks default sorting option") ?: "Title")
    }

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
                Column(
                    modifier = Modifier.padding(PaddingValues(start = 20.dp, top = 8.dp))
                ) {
                    Text(
                        text = "Notebooks default sorting option",
                        style = MaterialTheme.typography.h6
                    )

                    Spacer(modifier = Modifier.height(16.dp))

                    RadioButtonGroup(
                        options = listOf("Title", "Date created (asc)", "Date created (desc)"),
                        selectedOption = savedSortingOption,
                        onOptionSelected = { option ->
                            savedSortingOption = option
                            preferences.saveNew("Notebooks default sorting option", option)
                        }
                    )
                }
            }
        }
    )
}

@Composable
fun RadioButtonGroup(
    options: List<String>,
    selectedOption: String,
    onOptionSelected: (String) -> Unit
) {
    Column {
        options.forEach { option ->
            Row(
                verticalAlignment = androidx.compose.ui.Alignment.CenterVertically,
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 4.dp)
                    .clickable { onOptionSelected(option) }
            ) {
                RadioButton(
                    selected = selectedOption == option,
                    onClick = null // Set to null to avoid double invocation
                )
                Text(
                    text = option,
                    style = MaterialTheme.typography.body1,
                    modifier = Modifier.padding(start = 8.dp)
                )
            }
        }
    }
}
