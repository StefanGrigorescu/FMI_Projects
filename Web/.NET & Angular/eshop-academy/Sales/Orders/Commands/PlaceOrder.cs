using Hangfire.Mediator;
using Mediator;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace Sales.Orders.Commands
{
    public sealed class PlaceOrderController : ControllerBase
    {
        private readonly IMediator _mediator;

        public PlaceOrderController(IMediator mediator)
        {
            _mediator = mediator;
        }

        [HttpPost(SalesRoutes.Orders.Place, Name = "place-order")]
        [Tags(SalesRoutes.Orders.Tag)]
        public async Task<IActionResult> PlaceOrder([FromBody] PlaceOrderRequest request, CancellationToken cancellationToken = default)
        {
            PlaceOrderCommand command = PlaceOrderCommand.From(request);
            AppResponse<PlaceOrderResponse> response = AppResponse<PlaceOrderResponse>.Succeeded(new());//await _mediator.Send(command, cancellationToken);
            _mediator.Enqueue<PlaceOrderCommand, AppResponse<PlaceOrderResponse>>(command);
            
            return this.From(response);
        }
    }

    public sealed record PlaceOrderRequest
    {
        public Guid Id { get; init; }
    }

    public sealed record PlaceOrderResponse
    {

    }

    public sealed record PlaceOrderCommand : IRequest<AppResponse<PlaceOrderResponse>>
    {
        public required Guid Id { get; init; }

        private PlaceOrderCommand() { }

        public static PlaceOrderCommand From(PlaceOrderRequest source) => new()
        { 
            Id = source.Id
        };
    }

    public sealed class PlaceOrderCommandHandler : IRequestHandler<PlaceOrderCommand, AppResponse<PlaceOrderResponse>>
    {
        // Inject here exactly what you need to handle the request
        public PlaceOrderCommandHandler()
        {

        }

        public async ValueTask<AppResponse<PlaceOrderResponse>> Handle(PlaceOrderCommand request, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
            await Task.Delay(1, cancellationToken); // <- Replace this with actual handle logic

            return AppResponse<PlaceOrderResponse>.Succeeded(new()
            {

            });
        }
    }
}
