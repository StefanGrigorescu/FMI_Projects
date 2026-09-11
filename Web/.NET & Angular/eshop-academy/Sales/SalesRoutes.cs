namespace Sales
{
    public static class SalesRoutes
    {
        private const string _root = "api";

        private const string _base = _root + "/sales";

        public static class Orders
        {
            private const string _ordersRoute = _base + "/orders";

            public const string Tag = "Orders";

            public const string Place = _ordersRoute;
        }
    }
}
