namespace Shipping
{
    public abstract class EntityBase
    {
        public DateTime CreatedOn { get; set; }
        public DateTime? LastUpdatedOn { get; set; }
    }
}
