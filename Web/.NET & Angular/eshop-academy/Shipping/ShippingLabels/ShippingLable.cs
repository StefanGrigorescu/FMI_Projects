namespace Shipping.ShippingLabels
{
    public sealed class ShippingLable: EntityBase
    {
        public required ShippingLableId Id { get; init; }
        public required Guid OrderId { get; init; }

        private ShippingLable() { }

        public static ShippingLable From(Guid orderId) => new()
        {
            Id = new(Guid.NewGuid()),
            OrderId = orderId
        };
    }

    public sealed record ShippingLableId(Guid Id);
}
