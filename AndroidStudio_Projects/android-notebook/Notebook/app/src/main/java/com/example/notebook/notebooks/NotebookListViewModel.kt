package com.example.notebook.notebooks

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.notebook.data.INotebooksRepo
import com.example.notebook.data.NotebookEntity
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class NotebookListViewModel(
    private val repo: INotebooksRepo
): ViewModel() {
    private val _screenState = MutableStateFlow(NotebookListScreenState())
    private val _sortByState = MutableStateFlow(NotebooksSortBy.DateCreatedAsc)
    private val _searchTextState = MutableStateFlow("")

    private val _notebooksState: StateFlow<List<NotebookEntity>> = combine(_sortByState, _searchTextState)
        { sortBy, searchText ->
            withContext(Dispatchers.IO) {
                repo.getNotebookEntities(sortBy, searchText)
            }
    }.stateIn(viewModelScope, SharingStarted.WhileSubscribed(), emptyList())

    val screenState = combine(_screenState, _sortByState, _searchTextState, _notebooksState) { state, sortBy, searchText, notebooks ->
        state.copy(
            sortBy = sortBy,
            searchText = searchText,
            notebooks = notebooks
                .map { entity -> entity.toNotebook() },
        )
    }.stateIn(viewModelScope, SharingStarted.WhileSubscribed(stopTimeoutMillis = 5000), NotebookListScreenState())


    fun onEvent(event: NotebooksEvent): Unit {
        when(event) {
            is NotebooksEvent.SearchNotebookEvent -> {
                if(_searchTextState.value != event.searchText)
                _searchTextState.value = event.searchText
            }
            is NotebooksEvent.SortNotebooksEvent -> {
                _sortByState.value = event.sortBy
            }
            is NotebooksEvent.DeleteNotebookEvent -> {
                viewModelScope.launch {
                    repo.deleteById(event.id)
                }
            }
            is NotebooksEvent.ChangeNotebookPasswordEvent -> {

            }
            else -> {
                // Do nothing for other events
            }
        }
    }
}

enum class NotebooksSortBy {
    Id,
    Title,
    DateCreatedAsc,
    DateCreatedDesc;

    override fun toString(): String {
        return when (this) {
            Id -> "ID"
            Title -> "Title"
            DateCreatedAsc -> "Date created (asc)"
            DateCreatedDesc -> "Date created (desc)"
        }
    }
}

data class NotebookListScreenState(
    val notebooks: List<Notebook> = emptyList(),
    val sortBy: NotebooksSortBy = NotebooksSortBy.DateCreatedAsc,
    val searchText: String = ""
)
