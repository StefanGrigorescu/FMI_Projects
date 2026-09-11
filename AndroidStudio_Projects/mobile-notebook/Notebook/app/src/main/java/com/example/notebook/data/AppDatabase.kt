package com.example.notebook.data

import android.content.Context
import androidx.room.*
import java.time.OffsetDateTime

@Database(
    entities = [
        NotebookEntity::class,
        NoteEntity::class],
    version=2)
@TypeConverters(StringConverters::class)
abstract class AppDatabase: RoomDatabase() {
    abstract val notebooksDao: NotebooksDao
    abstract val notesDao: NotesDao

    companion object {
        private var Instance: AppDatabase? = null

        fun getInstace(context: Context): AppDatabase {
            if(Instance == null) {
                Instance = Room.databaseBuilder(context, AppDatabase::class.java, name = "notebook.db")
                    // can add migrations here
                    .fallbackToDestructiveMigration()
                    .build()
            }
            return Instance as AppDatabase
        }
    }
}

class StringConverters {
    @TypeConverter
    fun fromOffsetDateTime(value: OffsetDateTime?): String? {
        return value?.toString()
    }

    @TypeConverter
    fun toOffsetDateTime(value: String?): OffsetDateTime? {
        return value?.let { OffsetDateTime.parse(it) }
    }
}
