package com.example.notebook.notebooks

sealed interface NotebooksEvent {
    // NotebookListScreen Events:
    data class ChangeNotebookPasswordEvent(val newPassword: String, val confirmNewPassword: String): NotebooksEvent
    data class DeleteNotebookEvent(val id: Long?): NotebooksEvent

    data class SortNotebooksEvent(val sortBy: NotebooksSortBy): NotebooksEvent
    data class SearchNotebookEvent(val searchText: String): NotebooksEvent

    // CreateNotebookScreen and NotebookLockScreen Events:
    data class SubmitCreateNotebookFormEvent(val title: String, val description: String, val password: String?): NotebooksEvent
    data class SubmitNotebookLockFormEvent(val password: String): NotebooksEvent
    data class SetTitleEvent(val title: String): NotebooksEvent
    data class SetDescriptionEvent(val description: String): NotebooksEvent
    data class SetPasswordEvent(val password: String): NotebooksEvent
    data class SetConfirmPasswordEvent(val confirmPassword: String): NotebooksEvent
    data class SetIsLoadingEvent(val isLoading: Boolean): NotebooksEvent
    object ClearFormEvent: NotebooksEvent
}
