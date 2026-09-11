package com.example.notebook.data

import androidx.room.*
import kotlinx.coroutines.flow.Flow

@Dao
interface NotebooksDao {
    // Query Flows
    @Query("SELECT * FROM notebooks WHERE id = :id LIMIT 1")
    fun getOneByIdFlow(id: Long?): Flow<NotebookEntity?>


    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY id ASC")
    fun getFilteredOrderByIdFlow(searchText: String): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY LOWER(title) ASC")
    fun getFilteredOrderByTitleFlow(searchText: String): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY date_created ASC")
    fun getFilteredOrderByDateCreatedFlow(searchText: String): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY date_created DESC")
    fun getFilteredOrderByDateCreatedDescendingFlow(searchText: String): Flow<List<NotebookEntity>>


    @Query("SELECT * FROM notebooks ORDER BY id ASC")
    fun getAllOrderByIdFlow(): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks ORDER BY LOWER(title) ASC")
    fun getAllOrderByTitleFlow(): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks ORDER BY date_created ASC")
    fun getAllOrderByDateCreatedFlow(): Flow<List<NotebookEntity>>

    @Query("SELECT * FROM notebooks ORDER BY date_created DESC")
    fun getAllOrderByDateCreatedDescendingFlow(): Flow<List<NotebookEntity>>


    // Query Non-Flows
    @Query("SELECT * FROM notebooks WHERE id = :id LIMIT 1")
    fun getOneById(id: Long?): NotebookEntity?


    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY id ASC")
    fun getFilteredOrderById(searchText: String): List<NotebookEntity>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY LOWER(title) ASC")
    fun getFilteredOrderByTitle(searchText: String): List<NotebookEntity>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY date_created ASC")
    fun getFilteredOrderByDateCreated(searchText: String): List<NotebookEntity>

    @Query("SELECT * FROM notebooks WHERE (LOWER(title) LIKE LOWER('%' || :searchText || '%') OR LOWER(description) LIKE LOWER('%' || :searchText || '%')) ORDER BY date_created DESC")
    fun getFilteredOrderByDateCreatedDescending(searchText: String): List<NotebookEntity>


    @Query("SELECT * FROM notebooks ORDER BY id ASC")
    fun getAllOrderById(): List<NotebookEntity>

    @Query("SELECT * FROM notebooks ORDER BY LOWER(title) ASC")
    fun getAllOrderByTitle(): List<NotebookEntity>

    @Query("SELECT * FROM notebooks ORDER BY date_created ASC")
    fun getAllOrderByDateCreated(): List<NotebookEntity>

    @Query("SELECT * FROM notebooks ORDER BY date_created DESC")
    fun getAllOrderByDateCreatedDescending(): List<NotebookEntity>


    // Commands
    @Insert
    suspend fun insert(notebook: NotebookEntity): Unit

    @Update
    suspend fun update(notebook: NotebookEntity): Unit

    @Delete
    suspend fun delete(notebook: NotebookEntity): Unit

    @Query("DELETE FROM notebooks WHERE id = :id")
    suspend fun deleteById(id: Long?): Unit
}
