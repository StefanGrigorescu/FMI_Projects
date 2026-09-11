package com.example.notebook.data

import com.example.notebook.notes.NotesSortBy
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flowOf

class NotesRepo(
    private val dao: NotesDao
): INotesRepo {

    // Queries
    override fun getNoteEntities(
        notebookId: Long?,
        sortBy: NotesSortBy,
        searchText: String): List<NoteEntity>
    {
        if(searchText.isEmpty()) {
            return when (sortBy) {
                NotesSortBy.Id -> dao.getAllByNotebookIdOrderById(notebookId)
                NotesSortBy.Title -> dao.getAllByNotebookIdOrderByTitle(notebookId)
                NotesSortBy.DateCreatedAsc -> dao.getAllByNotebookIdOrderByDateCreated(notebookId)
                NotesSortBy.DateCreatedDesc -> dao.getAllByNotebookIdOrderByDateCreatedDescending(notebookId)
            }
        }

        return when(sortBy) {
            NotesSortBy.Id -> dao.getFilteredOrderById(notebookId, searchText)
            NotesSortBy.Title -> dao.getFilteredOrderByTitle(notebookId, searchText)
            NotesSortBy.DateCreatedAsc -> dao.getFilteredOrderByDateCreated(notebookId, searchText)
            NotesSortBy.DateCreatedDesc -> dao.getFilteredOrderByDateCreatedDescending(notebookId, searchText)
        }
    }

    override fun getNoteEntitiesFlow(
        notebookId: Long?,
        sortBy: NotesSortBy,
        searchText: String): Flow<List<NoteEntity>>
    {
        if(searchText.isEmpty()) {
            return when (sortBy) {
                NotesSortBy.Id -> dao.getAllByNotebookIdOrderByIdFlow(notebookId)
                NotesSortBy.Title -> dao.getAllByNotebookIdOrderByTitleFlow(notebookId)
                NotesSortBy.DateCreatedAsc -> dao.getAllByNotebookIdOrderByDateCreatedFlow(notebookId)
                NotesSortBy.DateCreatedDesc -> dao.getAllByNotebookIdOrderByDateCreatedDescendingFlow(notebookId)
            }
        }

        return when(sortBy) {
            NotesSortBy.Id -> dao.getFilteredOrderByIdFlow(notebookId, searchText)
            NotesSortBy.Title -> dao.getFilteredOrderByTitleFlow(notebookId, searchText)
            NotesSortBy.DateCreatedAsc -> dao.getFilteredOrderByDateCreatedFlow(notebookId, searchText)
            NotesSortBy.DateCreatedDesc -> dao.getFilteredOrderByDateCreatedDescendingFlow(notebookId, searchText)
        }
    }

    override fun getNoteEntityById(noteId: Long?): NoteEntity? {
        if(noteId == null) {
            return null
        }
        return dao.getOneById(noteId)
    }

    override fun getNoteEntityByIdFlow(noteId: Long?): Flow<NoteEntity?> {
        if(noteId == null) {
            return flowOf(null)
        }
        return dao.getOneByIdFlow(noteId)
    }


    // Commands
    override suspend fun insert(note: NoteEntity): Unit = dao.insert(note)

    override suspend fun update(note: NoteEntity): Unit = dao.update(note)

    override suspend fun delete(note: NoteEntity): Unit = dao.delete(note)

    override suspend fun deleteById(id: Long?): Unit = dao.deleteById(id)
}


interface INotesRepo {
    // Queries
    fun getNoteEntities(
        notebookId: Long?,
        sortBy: NotesSortBy,
        searchText: String): List<NoteEntity>

    fun getNoteEntitiesFlow(
        notebookId: Long?,
        sortBy: NotesSortBy,
        searchText: String): Flow<List<NoteEntity>>

    fun getNoteEntityById(noteId: Long?): NoteEntity?

    fun getNoteEntityByIdFlow(noteId: Long?): Flow<NoteEntity?>

    // Commands
    suspend fun insert(note: NoteEntity): Unit

    suspend fun update(note: NoteEntity): Unit

    suspend fun delete(note: NoteEntity): Unit

    suspend fun deleteById(id: Long?): Unit
}
