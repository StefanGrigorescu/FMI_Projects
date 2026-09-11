using Mediator;

namespace Hangfire.Mediator
{
    public static class MediatorExtensions
    {
        public static void Enqueue<TAppRequest, TAppResponse>(this IMediator mediator, TAppRequest request)
            where TAppRequest :  IRequest<TAppResponse>
        {
            BackgroundJobClient backgroundJobClient = new();
            backgroundJobClient.Enqueue<MediatorHangfireBridge>(bridge => bridge.SendAsync<TAppRequest, TAppResponse>(request));
        }
    }
}
