package com.example.notebook.data

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey
import com.example.notebook.notebooks.Notebook
import com.example.notebook.notes.Note
import java.time.OffsetDateTime
import java.time.ZoneOffset

@Entity(
    tableName = "notes",
    foreignKeys = [ForeignKey(
        entity = NotebookEntity::class,
        parentColumns = arrayOf("id"),
        childColumns = arrayOf("notebook_id"),
        onDelete = ForeignKey.CASCADE)]
)
data class NoteEntity(
    var title: String = "",
    var content: String = "",
    @ColumnInfo(name = "notebook_id", index = true)
    var notebookId: Long? = null,
) {
    @PrimaryKey(autoGenerate = true)
    var id: Long? = null

    @ColumnInfo(name = "date_created")
    var dateCreated: OffsetDateTime = OffsetDateTime.now(ZoneOffset.UTC)

    fun toNote(): Note {
        return Note(
            id = id,
            title = title,
        )
    }
}
