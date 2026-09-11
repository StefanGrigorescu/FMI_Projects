package com.example.notebook.notes

import androidx.compose.foundation.clickable
import androidx.compose.foundation.horizontalScroll
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.rememberScrollState
import androidx.compose.material.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.MoreVert
import androidx.compose.material.icons.filled.Search
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.example.notebook.AppBrand
import com.example.notebook.navigation.BottomBar
import com.example.notebook.navigation.Screen
import com.example.notebook.notebooks.NotebooksEvent
import org.koin.androidx.compose.getViewModel

@Composable
fun NoteListScreen(
    navController: NavHostController
) {
    val viewModel: NoteListViewModel = getViewModel<NoteListViewModel>()
    val notesState: NoteListScreenState by viewModel.screenState.collectAsState()

    Scaffold(
        topBar = {
            AppBrand(Screen.NoteList.screenName)
        },
        bottomBar = { BottomBar(navController = navController) },
        floatingActionButton = { AddNoteButton(viewModel.notebookId, navController) },
        floatingActionButtonPosition = FabPosition.End
    ) { padding ->
        LazyColumn(
            contentPadding = padding,
            modifier = Modifier.fillMaxSize().padding(PaddingValues(bottom = 16.dp)),
            verticalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            item {
                NoteSearchView(notesState.searchText) { event ->
                    viewModel.onEvent(event)
                }
            }
            item {
                NoteSortView(notesState) { event ->
                    viewModel.onEvent(event)
                }
            }
            if (notesState.notes.isEmpty()) {
                val noNotesYet = "There is no note created in this notebook yet. Try creating one!"

                item {
                    Text(
                        text = noNotesYet,
                        style = MaterialTheme.typography.h5,
                        modifier = Modifier.padding(PaddingValues(start = 20.dp, top=8.dp))
                    )
                }
            } else {
                items(items = notesState.notes) { note ->
                    NoteListItem(note, navController)
                }
            }
        }
    }
}

@Composable
fun AddNoteButton(
    notebookId: Long?,
    navController: NavHostController
) {
    val addNoteButtonDescription = "Add a new note"

    FloatingActionButton(
        onClick = {
            navController.navigate(Screen.CreateNote.routeFactory(notebookId))
        },
        content = { Icon(Icons.Filled.Add, contentDescription = addNoteButtonDescription) },
        modifier = Modifier
            .padding(PaddingValues(
                top = 12.dp,
                bottom = 12.dp,
                end = 24.dp
            ))
            .wrapContentSize()
    )
}

@Composable
fun NoteSearchView(
    stateSearchText: String,
    onSearchNotes: (NotesEvent.SearchNoteEvent) -> Unit
) {
    val searchNoteHint = "Search note by name"

    TextField(
        value = stateSearchText,
        onValueChange = { newSearchText: String ->
            onSearchNotes(NotesEvent.SearchNoteEvent(newSearchText))
        },
        label = { Text(searchNoteHint) },
        leadingIcon = {
            Icon(
                Icons.Filled.Search,
                contentDescription = null,
                modifier = Modifier
                    .clickable {
                        onSearchNotes(NotesEvent.SearchNoteEvent(stateSearchText))
                    },)
        },        modifier = Modifier.fillMaxWidth()
    )
}

@Composable
fun NoteSortView(
    notesState: NoteListScreenState,
    onSortNotes: (NotesEvent.SortNotesEvent) -> Unit
) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .horizontalScroll(rememberScrollState()),
        verticalAlignment = Alignment.CenterVertically
    ) {
        NotesSortBy.values().forEach { sortBy ->
            Row(
                modifier = Modifier
                    .clickable {
                        onSortNotes(NotesEvent.SortNotesEvent(sortBy))
                    },
                verticalAlignment = Alignment.CenterVertically
            ) {
                RadioButton(
                    selected = notesState.sortBy == sortBy,
                    onClick = {
                        onSortNotes(NotesEvent.SortNotesEvent(sortBy))
                    })
                Text(text = sortBy.toString())
            }
        }
    }
}

@Composable
fun NoteListItem(
    note: Note,
    navController: NavHostController
) {
    Row(
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(
            modifier = Modifier
                .padding(PaddingValues(start = 20.dp, ))
                .weight(1f)
                .clickable(onClick = {
                        // TODO: Uncomment the following line after adding the new screen and registering it in the screens enum
                        // navController.navigate(Screen.NoteScreen.route)
                    }
                ),
        ) {
            Text(
                text = note.title,
                style = MaterialTheme.typography.h6,
                modifier = Modifier.padding(bottom = 8.dp)
            )
        }
        IconButton(
            onClick = {
                // TODO: Open a "more options" dropdown for the item
            }) {
            Icon(imageVector = Icons.Default.MoreVert, contentDescription = "More options")
        }
    }
}
