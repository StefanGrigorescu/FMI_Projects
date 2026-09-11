using GeneticSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MFIS.Transitions
{
    internal static class TransitionsFactory
    {
        public static ConditionedTransition Accelerating(Gene gene) => new(
            Label.Accelerate,
            gene,
            new Interval()
            {
                LeftBoundary = 0,
                ClosedLeft = false
            });

        public static ConditionedTransition Decelerating(Gene gene) => new(
            Label.Accelerate,
            gene,
            new Interval()
            {
                RightBoundary = 0,
                ClosedRight = false
            });

        public static ConditionedTransition Maintaining(Gene gene) => new(
            Label.Accelerate,
            gene,
            new Interval()
            {
                LeftBoundary = 0,
                ClosedLeft = true,
                RightBoundary = 0,
                ClosedRight = true
            });

        public static NonConditionedTransition Stopping() => _stopping;

        private static NonConditionedTransition _stopping = new()
        {
            Label = Label.Stop
        };
    }
}
