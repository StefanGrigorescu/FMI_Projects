package com.example.notebook.data

import androidx.room.*
import kotlinx.coroutines.flow.Flow


@Dao
interface NotesDao {
    // Query Flows
    @Query("SELECT * FROM notes WHERE id = :id LIMIT 1")
    fun getOneByIdFlow(id: Long?): Flow<NoteEntity?>


    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY id ASC")
    fun getFilteredOrderByIdFlow(notebookId: Long?, searchText: String): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY LOWER(title) ASC")
    fun getFilteredOrderByTitleFlow(notebookId: Long?, searchText: String): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY date_created ASC")
    fun getFilteredOrderByDateCreatedFlow(notebookId: Long?, searchText: String): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY date_created DESC")
    fun getFilteredOrderByDateCreatedDescendingFlow(notebookId: Long?, searchText: String): Flow<List<NoteEntity>>


    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY id ASC")
    fun getAllByNotebookIdOrderByIdFlow(notebookId: Long?): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY LOWER(title) ASC")
    fun getAllByNotebookIdOrderByTitleFlow(notebookId: Long?): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY date_created ASC")
    fun getAllByNotebookIdOrderByDateCreatedFlow(notebookId: Long?): Flow<List<NoteEntity>>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY date_created DESC")
    fun getAllByNotebookIdOrderByDateCreatedDescendingFlow(notebookId: Long?): Flow<List<NoteEntity>>


    // Query Non-Flows
    @Query("SELECT * FROM notes WHERE id = :id LIMIT 1")
    fun getOneById(id: Long?): NoteEntity?


    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY id ASC")
    fun getFilteredOrderById(notebookId: Long?, searchText: String): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY LOWER(title) ASC")
    fun getFilteredOrderByTitle(notebookId: Long?, searchText: String): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY date_created ASC")
    fun getFilteredOrderByDateCreated(notebookId: Long?, searchText: String): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId AND LOWER(title) LIKE LOWER('%' || :searchText || '%') ORDER BY date_created DESC")
    fun getFilteredOrderByDateCreatedDescending(notebookId: Long?, searchText: String): List<NoteEntity>


    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY id ASC")
    fun getAllByNotebookIdOrderById(notebookId: Long?): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY LOWER(title) ASC")
    fun getAllByNotebookIdOrderByTitle(notebookId: Long?): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY date_created ASC")
    fun getAllByNotebookIdOrderByDateCreated(notebookId: Long?): List<NoteEntity>

    @Query("SELECT * FROM notes WHERE notebook_id=:notebookId ORDER BY date_created DESC")
    fun getAllByNotebookIdOrderByDateCreatedDescending(notebookId: Long?): List<NoteEntity>


    // Commands
    @Insert
    suspend fun insert(note: NoteEntity)

    @Update
    suspend fun update(note: NoteEntity)

    @Delete
    suspend fun delete(note: NoteEntity)

    @Query("DELETE FROM notes WHERE id = :id")
    suspend fun deleteById(id: Long?)
}
