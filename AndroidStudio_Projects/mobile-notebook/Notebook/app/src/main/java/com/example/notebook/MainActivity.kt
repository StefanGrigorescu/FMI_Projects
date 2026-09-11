package com.example.notebook

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Shadow
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import androidx.navigation.compose.rememberNavController
import com.example.notebook.navigation.SetupNavGraph
import com.example.notebook.ui.theme.GrayDefault
import com.example.notebook.ui.theme.NotebookTheme

class MainActivity : ComponentActivity() {
    lateinit var navController: NavHostController

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            NotebookTheme() {
                navController = rememberNavController()

                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background
                ) {
                    SetupNavGraph(navController = navController)
                }
            }
        }
    }
}

@Composable
fun AppBrand(screenName: String = "") {
    TopAppBar(
        modifier = Modifier.height(64.dp),
        title = {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 16.dp),
                horizontalArrangement = Arrangement.spacedBy(8.dp),
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = stringResource(id = R.string.app_name),
                    style = MaterialTheme.typography.h5.copy(
                        shadow = Shadow(
                            color = GrayDefault,
                            offset = Offset(x = 2f, y = 4f),
                            blurRadius = 0.1f
                        )
                    ),
                    fontWeight = FontWeight.SemiBold
                )

                Box(
                    modifier = Modifier
                        .height(24.dp)
                        .width(2.dp)
                        .background(GrayDefault)
                )

                Text(text = screenName, style = MaterialTheme.typography.h6)
            }},
        backgroundColor = MaterialTheme.colors.primary,
        elevation = 0.dp
    )
}
