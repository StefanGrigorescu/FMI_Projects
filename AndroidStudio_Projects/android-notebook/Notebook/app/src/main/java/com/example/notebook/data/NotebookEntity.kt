package com.example.notebook.data

import androidx.room.*
import com.example.notebook.notebooks.Notebook
import java.time.OffsetDateTime
import java.time.ZoneOffset

@Entity(tableName = "notebooks")
data class NotebookEntity(
    var title: String = "",
    var description: String = "",

    var password: String? = null
) {
    @PrimaryKey(autoGenerate = true)
    var id: Long? = null

    @ColumnInfo(name = "date_created")
    var dateCreated: OffsetDateTime = OffsetDateTime.now(ZoneOffset.UTC)

    fun toNotebook(): Notebook {
        return Notebook(
            id = id,
            title = title,
            description = description,
            hasPassword = !password.isNullOrEmpty()
        )
    }
}

data class NotebookWithNotes(
    @Embedded
    var notebook: NotebookEntity,
    @Relation(
        parentColumn = "id",
        entityColumn = "notebook_id"
    )
    var notes: List<NoteEntity>
)
