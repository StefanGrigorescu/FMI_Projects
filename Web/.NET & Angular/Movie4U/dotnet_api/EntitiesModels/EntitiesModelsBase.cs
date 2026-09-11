
namespace Movie4U.EntitiesModels
{
    /**<summary>
     * Abstract base class for pairs of Entities and Models.
     * </summary>>*/
    abstract public class EntitiesModelsBase<EntityType, ModelType>
    {        
        /**<summary>
        * Copies data from source Entity. 
        * Does not include reference values.
        * </summary> */
        abstract public void Copy(EntityType source);

        /**<summary>
        * Copies data from source Model. 
        * Does not include reference values.
        * </summary> */
        abstract public void Copy(ModelType source);


        /**<summary>
        * Copies data from source Model. 
        * It should include reference values too.
        * However, it calls its Copy couterpart by default, as overriding it is not mandatory (objects might not have reference types).
        * </summary> */
        virtual public void ShallowCopy(ModelType source) 
        {
            Copy(source);
        }
    }
}
