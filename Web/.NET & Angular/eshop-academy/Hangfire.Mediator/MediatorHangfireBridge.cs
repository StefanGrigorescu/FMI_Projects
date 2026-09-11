using Mediator;

namespace Hangfire.Mediator
{
    public class MediatorHangfireBridge
    {
        private readonly IMediator _mediator;

        public MediatorHangfireBridge(IMediator mediator)
        {
            _mediator = mediator;
        }

        public async Task SendAsync<TAppRequest, TAppResponse>(TAppRequest request)
            where TAppRequest : IRequest<TAppResponse>
        {
            await _mediator.Send(request);
        }
    }
}
