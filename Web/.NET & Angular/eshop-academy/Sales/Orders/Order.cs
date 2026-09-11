namespace Sales.Orders
{
    public sealed class Order: EntityBase
    {
        public required OrderId Id { get; init; }
    }

    public sealed record OrderId(Guid Id);
}
