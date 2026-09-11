using Sales.Contracts.Orders;

namespace Shipping.ShippingLabels.Commands
{
    public sealed class CreateShippingLabel
    {
        //private readonly ILogger<CreateShippingLabel> _logger;

        //public CreateShippingLabel(ILogger<CreateShippingLabel> logger)
        //{
        //    _logger = logger;
        //}

        public void Handle(OrderPlacedEvent orderPlacedEvent)
        {
            // Make a call to own db and fetch a previously populated information (via an http call) 
            // containing the same orderId and the other data necessary shipping specific data (like shipping address). 

            //_logger.LogInformation($"Order with id {orderPlacedEvent.OrderId} has created a shipping label.");
        }
    }
}
