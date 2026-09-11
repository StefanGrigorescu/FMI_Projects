using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Finance
{
    public readonly struct AppResponse<T>
    {
        public T? Data { get; init; }
        public required IEnumerable<string> ErrorMessages { get; init; }
        public FailureReason? FailureReason { get; init; }
        public required bool Success { get; init; }

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="true"/>,
        /// the <see cref="Data"/> property to the <paramref name="data"/> parameter
        /// and the <see cref="ErrorMessages"/> property to empty.
        /// </summary>
        public static AppResponse<T> Succeeded(T data)
            => new()
            {
                Success = true,
                Data = data,
                ErrorMessages = Array.Empty<string>(),
                FailureReason = null,
            };

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="false"/>,
        /// the <see cref="Data"/> property to <see langword="null"/>
        /// and the <see cref="ErrorMessages"/> property to the <paramref name="errorMessages"/>.
        /// </summary>
        public static AppResponse<T> Failed(
            IEnumerable<string> errorMessages,
            FailureReason reason = Finance.FailureReason.BadRequest)
            => new()
            {
                Success = false,
                Data = default,
                ErrorMessages = errorMessages,
                FailureReason = reason,
            };

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="false"/>,
        /// the <see cref="Data"/> property to <see langword="null"/>
        /// and the <see cref="ErrorMessages"/> property to a new array only containing the <paramref name="errorMessage"/> parameter.
        /// </summary>
        public static AppResponse<T> Failed(
            string errorMessage,
            FailureReason reason = Finance.FailureReason.BadRequest)
            => new()
            {
                Success = false,
                Data = default,
                ErrorMessages = new[] { errorMessage },
                FailureReason = reason,
            };
    }


    public readonly struct AppResponse
    {
        public required IEnumerable<string> ErrorMessages { get; init; }
        public FailureReason? FailureReason { get; init; }
        public required bool Success { get; init; }

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="true"/>
        /// and the <see cref="ErrorMessages"/> property to empty.
        /// </summary>
        public static AppResponse Succeeded()
            => new()
            {
                Success = true,
                ErrorMessages = Array.Empty<string>(),
                FailureReason = null,
            };

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="false"/>
        /// and the <see cref="ErrorMessages"/> property to the <paramref name="errorMessages"/>.
        /// </summary>
        public static AppResponse Failed(
            IEnumerable<string> errorMessages,
            FailureReason reason = Finance.FailureReason.BadRequest)
            => new()
            {
                Success = false,
                ErrorMessages = errorMessages,
                FailureReason = reason,
            };

        /// <summary>
        /// Called for succeeded actions.
        /// Sets the <see cref="Success"/> property to <see langword="false"/>
        /// and the <see cref="ErrorMessages"/> property to a new array only containing the <paramref name="errorMessage"/> parameter.
        /// </summary>
        public static AppResponse Failed(
            string errorMessage,
            FailureReason reason = Finance.FailureReason.BadRequest)
            => new()
            {
                Success = false,
                ErrorMessages = new[] { errorMessage },
                FailureReason = reason,
            };
    }


    public static class ErrorMessagesExtensions
    {
        /// <summary>
        /// If <paramref name="response"/> status is not Success, add all its error messages to the <paramref name="errorMessages"/> list.
        /// </summary>
        /// <typeparam name="T">The innter type of the <see cref="AppResponse{T}"/> instance.</typeparam>
        /// <param name="errorMessages">The initial error messages list.</param>
        /// <param name="response">The response object to check.</param>
        /// <returns>The same <paramref name="errorMessages"/> instance, 
        /// after conditionally appending <paramref name="response"/>'s error messages to it.</returns>
        public static List<string> AddRangeErrorMessagesIfFailed<T>(this List<string> errorMessages, AppResponse<T> response)
        {
            if (!response.Success)
            {
                errorMessages.AddRange(response.ErrorMessages);
            }

            return errorMessages;
        }

        /// <summary>
        /// If <paramref name="response"/> status is not Success, add all its error messages to the <paramref name="errorMessages"/> list.
        /// </summary>
        /// <typeparam name="T">The innter type of the <see cref="AppResponse"/> instance.</typeparam>
        /// <param name="errorMessages">The initial error messages list.</param>
        /// <param name="response">The response object to check.</param>
        /// <returns>The same <paramref name="errorMessages"/> instance, 
        /// after conditionally appending <paramref name="response"/>'s error messages to it.</returns>
        public static List<string> AddRangeErrorMessagesIfFailed(this List<string> errorMessages, AppResponse response)
        {
            if (!response.Success)
            {
                errorMessages.AddRange(response.ErrorMessages);
            }

            return errorMessages;
        }
    }


    public enum FailureReason
    {
        BadRequest = 0,
        Validation = 1,
        Unauthorized
    }
}
